#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>
#include <getopt.h>

#include "file.h"
#include "common.h"
#include "parse.h"

void print_usage(char *argv[]) {
  printf("====================================================");
  printf("Usage: %s -n -f <database file>\n", argv[0]); 
  printf("\t -n  -  create new database file\n");
  printf("\t -f  -  (required) path to database file\n");
  printf("====================================================");
}

int main(int argc, char *argv[]) { 
  int c; 
  bool newfile = false; 
  bool listing = false;
  bool total = false;
  char* filepath = NULL;
  char* addstring = NULL;
  char* delname = NULL;
  char* upstring = NULL;
  int dbfd = -1;
  struct dbheader_t* dbhdr = NULL;
  struct employee_t* employees = NULL;

  while((c = getopt(argc, argv, "nf:a:ltd:u:")) != -1) {
    switch(c) {
      case 'n':
        newfile = true; 
        break; 

      case 'f':
        filepath = optarg;
        break;

      case 'a': 
        addstring = optarg;
        break;

      case 'l':
        listing = true;
        break;

      case 't':
        total = true;
        break;

      case 'd': 
        delname = optarg; 
        break;

      case 'u':
        upstring = optarg; 
        break;

      case '?':
        printf("unknown option- %c\n", c); 
        break;

      default:
        return -1;
    }
  }


  if(filepath == NULL) {
    printf("Filepath is a required argument!\n");
    print_usage(argv);
    return 0;
  }

  if(newfile) {
    dbfd = create_db_file(filepath);

    if(dbfd == STATUS_ERROR) {
      printf("Unable to create database file\n");
      return -1; 
    }

    if(create_db_header(dbfd, &dbhdr) == STATUS_ERROR) {
      printf("Failed to create Database header\n");
      return -1;
    }
  }else{
    dbfd = open_db_file(filepath);

    if(dbfd == STATUS_ERROR) {
      printf("Unable to open database file\n");
      return -1; 
    }

    if(validate_db_header(dbfd, &dbhdr) == STATUS_ERROR) {
      printf("Failed to validate Database error\n");
      return -1;
    }
  }
  
  printf("Newfile: %d\n", newfile); 
  printf("Filepath: %s\n", filepath);

  if(read_employees(dbfd, dbhdr, &employees) != STATUS_SUCCESS) {
    printf("Failed to read employees\n");
    return 0;
  }

  if(addstring) {
    dbhdr -> count ++; 
    employees = (struct employee_t*) realloc(employees, dbhdr -> count * (sizeof(struct employee_t)));
    add_record(dbhdr, employees, addstring);
  }

  if(total) {
    num_employees(dbhdr);
  }

  if(delname) {
    if(del_employee_name(dbhdr, employees, delname) == -1) {
      printf("No employee with name %s found\n", delname);
    }
    employees = (struct employee_t*)realloc(employees, dbhdr -> count * sizeof(struct employee_t));
    printf("Employee with name %s deleted, remaining %d employees\n", delname, dbhdr -> count);
    num_employees(dbhdr);
  }

  if(upstring) {
    if(update_hours(dbhdr, employees, upstring) == STATUS_ERROR) {
      printf("Update failed!\n"); 
    }
  }

  if(listing) {
    list_employees(dbhdr, employees); 
  }


  output_file(dbfd, dbhdr, employees);

  printf("====================================================");
  printf("====================================================\n");
  return 0;
}
