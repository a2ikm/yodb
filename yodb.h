#ifndef __YODB_H__
#define __YODB_H__

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

void print_constants();
void print_leaf_node(void *node);

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

typedef struct {
  uint32_t id;
  char username[COLUMN_USERNAME_SIZE + 1];
  char email[COLUMN_EMAIL_SIZE + 1];
} Row;

extern const uint32_t ID_SIZE;
extern const uint32_t USERNAME_SIZE;
extern const uint32_t EMAIL_SIZE;

void serialize_row(Row *source, void *destination);
void deserialize_row(void *source, Row *destination);

#define TABLE_MAX_PAGES 100

typedef struct {
  int file_descriptor;
  uint32_t file_length;
  uint32_t num_pages;
  void *pages[TABLE_MAX_PAGES];
} Pager;

Pager *pager_open(const char *filename);
void pager_flush(Pager *pager, uint32_t page_num);
void *get_page(Pager *pager, uint32_t page_num);

typedef struct {
  uint32_t num_rows;
  Pager *pager;
  uint32_t root_page_num;
} Table;

typedef struct {
  Table *table;
  uint32_t page_num;
  uint32_t cell_num;
  bool end_of_table; // Indicates whether the table has a record here or not
} Cursor;

typedef enum {
  NODE_INTERNAL,
  NODE_LEAF
} NodeType;

NodeType *node_type_ptr(void *node);

uint32_t *leaf_node_num_cells_ptr(void *node);
void *leaf_node_cell(void *node, uint32_t cell_num);
uint32_t *leaf_node_key_ptr(void *node, uint32_t cell_num);
void *leaf_node_value_ptr(void *node, uint32_t cell_num);
void leaf_node_initialize(void *node);
Cursor *leaf_node_find(Table *table, uint32_t page_num, uint32_t key);
void leaf_node_insert(Cursor *cursor, uint32_t key, Row *row);

Table *db_open(const char *filename);
void db_close(Table *table);

Cursor *table_start(Table *table);
Cursor *table_find(Table *table, uint32_t key);

// Returns a pointer which the cursor points.
void *cursor_value(Cursor *cursor);
void cursor_advance(Cursor *cursor);

typedef enum {
  STATEMENT_INSERT,
  STATEMENT_SELECT
} StatementType;

typedef struct {
  StatementType type;
  Row row_to_insert;
} Statement;

typedef enum {
  EXECUTE_SUCCESS,
  EXECUTE_TABLE_FULL,
  EXECUTE_DUPLICATE_KEY
} ExecuteResult;

ExecuteResult execute_insert(Statement *statement, Table *table);
ExecuteResult execute_select(Statement *statement, Table *table);
ExecuteResult execute_statement(Statement *statement, Table *table);

#endif
