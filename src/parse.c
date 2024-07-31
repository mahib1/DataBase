#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "common.h"
#include "parse.h"

char* strip(char* str) {
    char* end;

    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)  
        return str;

    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';

    return str;
}

int update_hours(struct dbheader_t* dbhdr, struct employee_t* employees, char* upstring) {
  if(dbhdr == NULL) {
    printf("Improper header!\n"); 
    return STATUS_ERROR;
  }

  char* name = strip(strtok(upstring, ","));
  char* new_hours = strip(strtok(NULL, ",")); 

  for(int i = 0; i < dbhdr -> count; i++) {
    if(strcmp(employees[i].name , name) == 0) {
      int old_hours = employees[i].hours;
      employees[i].hours = atoi(new_hours);
      printf("Hours for %s successfully changed from %d to %d\n", name, old_hours, employees[i].hours);
      return STATUS_SUCCESS;
    }
  }
  printf("No such record with name %s\n", name);
  return STATUS_ERROR;
}

int del_employee_name(struct dbheader_t *dbhdr, struct employee_t *employees, const char *name) {
  for(int i = 0 ; i < dbhdr -> count; i++) {
    if(strcmp(employees[i].name , name) == 0) {
      for(int j = i; j < dbhdr -> count - 1; j++) {
        employees[j] = employees[j+1];
      }

      dbhdr -> count--; 
      return 0;
    }
  }

  return -1;

}

void num_employees(struct dbheader_t *dbhdr) {
  if(dbhdr == NULL) {
    printf("Improper Header!\n");
  }

  printf("Number of employees : %d\n", dbhdr -> count);
}

void list_employees(struct dbheader_t *dbhdr, struct employee_t *employees) {
  for(int i = 0 ; i < dbhdr -> count; i++) {
    printf("Employee : %d\n", i+1); 
    printf("\t%s, %s, %d\n", employees[i].name, employees[i].address, employees[i].hours);
  }
  return;
}

int add_record(struct dbheader_t *dbhdr, struct employee_t *employees, char *addstring) {
  printf("%s\n", addstring);
  char* name = strip(strtok(addstring, ","));
  char* addr = strip(strtok(NULL, ","));
  char* hours = strip(strtok(NULL, ","));

  printf("name : %s\n", name);
  printf("address : %s\n", addr);
  printf("hours : %s\n", hours);

  employees[dbhdr -> count - 1].hours = atoi(hours);

  strncpy(employees[dbhdr -> count -1].name, name, sizeof(employees[dbhdr -> count - 1].name));
  strncpy(employees[dbhdr -> count -1].address , addr, sizeof(employees[dbhdr -> count - 1].address));
   

  return STATUS_SUCCESS;

}

int read_employees(int fd, struct dbheader_t *dbhdr, struct employee_t **employeesOut) {
  if(fd < 0) {
    printf("Improper File Descriptor\n");
    return STATUS_ERROR;
  }

  int count = dbhdr -> count;

  struct employee_t* employees = (struct employee_t*) calloc(count, sizeof(struct employee_t));

  if(employees == NULL) {
    printf("Malloc failed\n");
    return STATUS_ERROR;
  }

  read(fd, employees, count * sizeof(struct employee_t));

  int i = 0 ; 
  for(; i < count ; i++) {
    employees[i].hours = ntohl(employees[i].hours);
  }

  *employeesOut = employees;
  return STATUS_SUCCESS;
}

int output_file(int fd, struct dbheader_t *dbhdr, struct employee_t* employees) {
  if(fd < 0) {
    printf("Improper File Descriptor\n");
    return STATUS_ERROR;
  }

  int realcount = dbhdr -> count;

	dbhdr->magic = htonl(dbhdr->magic);
	dbhdr->filesize = htonl((realcount * sizeof(struct employee_t)) + sizeof(struct dbheader_t));
	dbhdr->count = htons(dbhdr->count);
	dbhdr->version = htons(dbhdr->version);
  
  lseek(fd, 0, SEEK_SET);
  write(fd, dbhdr, sizeof(struct dbheader_t));

  for(int i = 0; i < realcount ; i++) {
    employees[i].hours = htonl(employees[i].hours);
    if(write(fd, &employees[i], sizeof(struct employee_t)) == -1) {
      perror("write"); 
      return STATUS_ERROR;
    }
  }

  off_t new_size = ntohl(dbhdr->filesize);
  if(ftruncate(fd, new_size) != 0) {
    perror("ftruncate");
    return STATUS_ERROR;
  }

  return STATUS_SUCCESS;
}	

int validate_db_header(int fd, struct dbheader_t **headerOut) {
  if(fd < 0) {
    printf("Invalid Descriptor!\n");
    return STATUS_ERROR;
  }
  
  struct dbheader_t *header = (struct dbheader_t*) calloc(1, sizeof(struct dbheader_t));
  if(header == NULL) {
    printf("Malloc failed to create a Database header\n");
    return STATUS_ERROR;
  }

  if(read(fd, header, sizeof(struct dbheader_t)) != sizeof(struct dbheader_t)){
    perror("Read");
    free(header); 
    return STATUS_ERROR;
  }

  header -> version = ntohs(header -> version);
  header -> count = ntohs(header -> count);
  header -> magic = ntohl(header -> magic);
  header -> filesize = ntohl(header -> filesize);

  if(header -> magic != HEADER_MAGIC) {
    printf("Improper header magic\n");
    free(header); 
    return STATUS_ERROR;
  }

  if(header -> version != 1) {
    printf("Improper header version\n");
    free(header); 
    return STATUS_ERROR;
  }

  struct stat dbstat  = {0};
  fstat(fd, &dbstat); 
  if(header -> filesize != dbstat.st_size) {
    printf("Corrupted file \n");
    free(header); 
    return -1;
  }
  
  *headerOut = header;

  return STATUS_SUCCESS;

}

int create_db_header(int fd, struct dbheader_t **headerOut) {	
  struct dbheader_t *header = (struct dbheader_t*) calloc(1,sizeof(struct dbheader_t)); 

  if(header == NULL) {
    printf("Malloc failed to create Database header\n"); 
    return STATUS_ERROR;
  }

  header -> version = 1; 
  header -> count = 0;

  header -> magic = HEADER_MAGIC; 
  header -> filesize = sizeof(struct dbheader_t);

  *headerOut = header;
  free(header);
  return STATUS_SUCCESS;
}


