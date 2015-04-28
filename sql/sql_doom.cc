#include "sql_doom.h"

bool doom_init_result(THD *thd)
{
  List<Item> field_list;
  Protocol *protocol= thd->protocol;
  DBUG_ENTER("doom_init_result");

  field_list.push_back(new Item_empty_string("sql",10));
  field_list.push_back(new Item_return_int("result",1, MYSQL_TYPE_LONG));
  field_list.push_back(new Item_empty_string("comment", DOOM_COMMENT_LEN));

  if (protocol->send_result_set_metadata(&field_list,
                            Protocol::SEND_NUM_ROWS | Protocol::SEND_EOF)) {

    thd->killed= THD::KILL_CONNECTION;
    DBUG_RETURN(TRUE);
  } else {
    DBUG_RETURN(FALSE);
  }
}

bool doom_add_result(THD *thd, const char* sql, int result, const char * comment)
{
  Protocol *protocol= thd->protocol;
  DBUG_ENTER("doom_add_result");

  protocol->prepare_for_resend();
  protocol->store(sql, system_charset_info);
  protocol->store_long(result);
  protocol->store(comment, system_charset_info);
  if (protocol->write()) {
    thd->killed= THD::KILL_CONNECTION;
    DBUG_RETURN(TRUE);
  }
  else {
    DBUG_RETURN(FALSE);
  }
}

void doom_end_result(THD *thd)
{
  /* if there is a error, we just add the error to the result */
  if (thd->get_stmt_da()->is_error()) {
    doom_add_result(thd, thd->query(), 1, thd->get_stmt_da()->message());
    thd->get_stmt_da()->reset_diagnostics_area();
  }

  my_eof(thd);
}
