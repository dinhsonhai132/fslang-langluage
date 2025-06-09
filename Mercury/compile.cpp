#include "../Include/compile.h"
bool skip = false;

__compiler_u compiler_init(void) {
  __compiler_u glb;
  glb.address = 0x0000;
  glb.byte = 0x0000;
  glb.line = 0x0000;
  glb.cid = 0x0000;
  glb.name = nullptr;
  glb.type = nullptr;
  glb.file = "stdin";
  return glb;
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_id(mAST_T *ast, __compiler_u &glb) {
  if (!ast) return NULL_CODE;

  if (ast->type == Identifier_)             return MerCompiler_compile_ast_identifier(ast, glb);
  if (ast->type == Literal)                 return MerCompiler_compile_ast_literal(ast, glb);
  if (ast->type == BinaryExpression)        return MerCompiler_compile_ast_binary_expression(ast, glb);
  if (ast->type == FunctionCallExpression)  return MerCompiler_compile_ast_function_call(ast, glb);
  if (ast->type == IfStatement)             return MerCompiler_compile_ast_if(ast, glb);
  if (ast->type == LetStatement)            return MerCompiler_compile_ast_let(ast, glb);
  if (ast->type == ReturnStatement)         return MerCompiler_compile_ast_return(ast, glb);
  if (ast->type == ExpressionStatement)     return MerCompiler_compile_ast_id_expression_statment(ast, glb);
  if (ast->type == FunctionStatement)       return MerCompiler_compile_ast_function(ast, glb); 
  if (ast->type == ComparisonExpression)    return MerCompiler_compile_ast_comparison_expression(ast, glb);
  if (ast->type == AssignStatement)         return MerCompiler_compile_ast_assign(ast, glb);
  if (ast->type == WhileStatement)          return MerCompiler_compile_ast_while(ast, glb);
  if (ast->type == StringIdentifier)        return MerCompiler_compile_ast_string_identifier(ast, glb);
  if (ast->type == ListStatement)           return MerCompiler_compile_ast_list(ast, glb);
  if (ast->type == ArrayExpression)         return MerCompiler_compile_ast_array(ast, glb);
  if (ast->type == ExtractExpression)       return MerCompiler_compile_ast_extract(ast, glb);
  if (ast->type == StoreIndexStatement)     return MerCompiler_compile_ast_store_index_statement(ast, glb);
  if (ast->type == ForInStatement)          return MerCompiler_compile_ast_for_in_statement(ast, glb);
  if (ast->type == LoopStatement)           return MerCompiler_compile_ast_loop(ast, glb);

  return NULL_CODE;
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_loop(mAST_T *ast, __compiler_u &glb) {
  __Mer_return_Code result = NULL_CODE;
  __Mer_return_Code body = NULL_CODE;

  for (auto &node : ast->do_body) {
    __Mer_return_Code _code = MerCompiler_compile_ast_id(node, glb);
    INSERT_VEC(body, _code);
    _code.prg_code.buff.clear();
  }

  ++glb.address;

  PUSH(result, CADDRESS);
  PUSH(result, glb.address);
  INSERT_VEC(result, body);
  PUSH(result, CJUMP_TO);
  PUSH(result, glb.address); 

  return result;
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_store_index_statement(mAST_T *ast, __compiler_u &glb) {
  __Mer_return_Code result = NULL_CODE;
  __Mer_return_Code extract_val_assign = MerCompiler_compile_ast_id(ast->extract_assign, glb);
  __Mer_return_Code extract_index = MerCompiler_compile_ast_id(ast->extract_value, glb);

  Mer_uint8_t address = NULL_UINT_8_T;
  bool found = false;

  for (auto &item : GLOBAL_TABLE) {
    if (item->__name == ast->extract_name) {
      address = item->__Address;
      found = true;
      break;
    }
  }

  if (!found) {
    MerDebug_system_error(COMPILER_ERROR, "List not found, can not extract value from list", glb.file);
  }

  if (!ast->extract_assign) {
    MerDebug_system_error(COMPILER_ERROR, "Assign value is null", glb.file);
  }

  if (!ast->extract_value) {
    MerDebug_system_error(COMPILER_ERROR, "Extract value is null", glb.file);
  }

  PUSH(result, CLOAD_GLOBAL);
  PUSH(result, address);
  INSERT_VEC(result, extract_index);
  INSERT_VEC(result, extract_val_assign);
  PUSH(result, CSTORE_INDEX);

  return result;
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_extract(mAST_T *ast, __compiler_u &glb) {
  __Mer_return_Code result = NULL_CODE;

  bool found = false;
  Mer_uint8_t list_address = NULL_UINT_8_T;

  for (auto &global : GLOBAL_TABLE) {
    if (global->__name == ast->extract_name) {
      found = true;
      list_address = global->__Address;
      break;
    }
  }

  if (!found) {
    MerDebug_system_error(COMPILER_ERROR, "List not found, can not extract value from list", glb.file);
  }

  __Mer_return_Code _code = MerCompiler_compile_ast_id(ast->extract_value, glb);
  PUSH(result, CLOAD_GLOBAL);
  PUSH(result, list_address);

  INSERT_VEC(result, _code);
  PUSH(result, CGET_ITEM);

  return result;
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_array(mAST_T *ast, __compiler_u &glb) {
  __Mer_return_Code result = NULL_CODE;
  __Mer_return_Code array = NULL_CODE;

  if (ast->type != ArrayExpression) {
    MerDebug_system_error(COMPILER_ERROR, "Invalid creating array", glb.file);
  }
  
  if (!ast->is_alone_val) {
    MerDebug_system_error(COMPILER_ERROR, "Invalid creating array", glb.file);
  }

  Mer_uint8_t size = ast->list.size();
  Mer_size_t ssize = size;

  for (auto &item : ast->list) {
    __Mer_return_Code _code = MerCompiler_compile_ast_id(item, glb);
    INSERT_VEC(array, _code);
  }

  INSERT_VEC(result, array);
  PUSH(result, CBUILD_LIST);
  PUSH(result, size);

  #ifdef SYSTEM_TEST
  cout << "[compiler.cpp] [MerCompiler_compile_ast_array] [pass]" << endl;
  #endif
  return result;
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_list(mAST_T *ast, __compiler_u &glb) {
  __Mer_return_Code result = NULL_CODE;
  __Mer_return_Code list = NULL_CODE;

  if (ast->type != ListStatement) {
    MerDebug_system_error(COMPILER_ERROR, "Invalid creating list", glb.file);
  }

  Mer_uint8_t size = ast->list.size();
  Mer_uint8_t address = ++glb.address;

  for (auto &item : ast->list) {
    __Mer_return_Code _code = MerCompiler_compile_ast_id(item, glb);
    INSERT_VEC(list, _code);
  }

  INSERT_VEC(result, list);
  PUSH(result, CBUILD_LIST);
  PUSH(result, size);
  PUSH(result, CSTORE_GLOBAL);
  PUSH(result, address);

  GLOBAL_TABLE.push_back(CREAT_GLOBAL_TABLE(address, ast->list_name.c_str(), ast->list_name));


  #ifdef SYSTEM_TEST
  cout << "[compiler.cpp] [MerCompiler_compile_ast_list] [pass]" << endl;
  #endif
  return result;
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_string_identifier(mAST_T *ast, __compiler_u &glb) {
  __Mer_return_Code result = NULL_CODE;
  __Mer_return_Code str = NULL_CODE;
  
  if (ast->type != StringIdentifier) {
    MerDebug_system_error(COMPILER_ERROR, "Invalid creating string identifier", glb.file);
  }

  Mer_uint8_t size = ast->str_v.size();
  
  vector<Mer_uint8_t> str_v;
  for (auto &item : ast->str_v) {
    str_v.push_back(item);
  }

  str.prg_code.buff.insert(str.prg_code.buff.end(), str_v.begin(), str_v.end());

  PUSH(result, CPUSH_NORMAL_MODE);
  PUSH(result, size);
  INSERT_VEC(result, str);

  #ifdef SYSTEM_TEST
  cout << "[compiler.cpp] [MerCompiler_compile_ast_string_identifier] [pass]" << endl;
  #endif
  return result;
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_assign(mAST_T *ast, __compiler_u &glb) {
  __Mer_return_Code _ass = NULL_CODE;
  if (!ast->assign_value) {
    MerDebug_system_error(COMPILER_ERROR, "Assign value is null", glb.file);
  }

  __Mer_return_Code result = NULL_CODE;
  Mer_uint8_t address;
  bool found = false;

  for (auto &item : GLOBAL_TABLE) {
    if (item->__name == ast->assign_iden) {
      address = item->__Address;
      found = true;
      break;
    }
  }

  if (!found) {
    Mer_real_string str = ast->assign_iden.c_str();
    Mer_real_string msg = "Variable not found " + str;
    MerDebug_system_error(COMPILER_ERROR, msg.c_str(), glb.file);
    return NULL_CODE;
  }

  if (ast->is_assign_operator) {
    Mer_uint8_t opcode = NULL_UINT_8_T;
    if (ast->is_plus_assign) {
      opcode = CBINARY_ADD;
    } else if (ast->is_minus_assign) {
      opcode = CBINARY_SUB;
    } else if (ast->is_mul_assign) {
      opcode = CBINARY_MUL;
    } else if (ast->is_div_assign) {
      opcode = CBINARY_DIV;
    } else if (ast->is_mod_assign) {
      opcode = CBINARY_MOD;
    } else {
      MerDebug_system_error(COMPILER_ERROR, "Invalid assign operator", glb.file);
    }

    PUSH(_ass, CLOAD_GLOBAL);
    PUSH(_ass, address);

    __Mer_return_Code _code = MerCompiler_compile_ast_id(ast->assign_value, glb);
    INSERT_VEC(_ass, _code);

    PUSH(_ass, opcode);

    INSERT_VEC(result, _ass);
    PUSH(result, CSTORE_GLOBAL);
    PUSH(result, address);
  } else {
    _ass = MerCompiler_compile_ast_id(ast->assign_value, glb);
    INSERT_VEC(result, _ass);
    PUSH(result, CSTORE_GLOBAL);
    PUSH(result, address);
  }

  #ifdef SYSTEM_TEST
  cout << "[compiler.cpp] [MerCompiler_compile_ast_assign] [pass]" << endl;
  #endif
  return result;
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_while(mAST_T *ast, __compiler_u &glb) {
  __Mer_return_Code result = NULL_CODE;
  __Mer_return_Code while_cond = MerCompiler_compile_ast_id(ast->while_cond, glb);
  __Mer_return_Code while_body = NULL_CODE;

  for (auto child : ast->while_body) {
    __Mer_return_Code _code = MerCompiler_compile_ast_id(child, glb);
    INSERT_VEC(while_body, _code);
    _code.prg_code.buff.clear();
  }
  
  Mer_uint8_t while_begin = ++glb.address;
  Mer_uint8_t while_end = ++glb.address;

  PUSH(result, CADDRESS);
  PUSH(result, while_begin);
  INSERT_VEC(result, while_cond);
  PUSH(result, CPOP_JUMP_IF_FALSE);
  PUSH(result, while_end);
  INSERT_VEC(result, while_body);
  PUSH(result, CJUMP_TO);
  PUSH(result, while_begin);
  PUSH(result, CADDRESS);
  PUSH(result, while_end);

  #ifdef SYSTEM_TEST
  cout << "[compiler.cpp] [MerCompiler_compile_ast_while] [pass]" << endl;
  #endif
  
  return result;
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_for_in_statement(mAST_T *ast, __compiler_u &glb) {
  __Mer_return_Code result = NULL_CODE;
  __Mer_return_Code for_body = NULL_CODE;
  __Mer_return_Code for_loop_iter = NULL_CODE;
  
  Mer_uint8_t __iter = ++glb.address;
  Mer_uint8_t __i = ++glb.address;
  Mer_uint8_t __value = ++glb.address;

  Mer_uint8_t loop_start = ++glb.address;
  Mer_uint8_t loop_end = ++glb.address;

  GLOBAL_TABLE.push_back(CREAT_GLOBAL_TABLE(__iter, "__iter", "__iter"));
  int __iter_idx = GLOBAL_TABLE.size() - 1;

  GLOBAL_TABLE.push_back(CREAT_GLOBAL_TABLE(__i, "__i", "__i"));
  int __i_idx = GLOBAL_TABLE.size() - 1;

  GLOBAL_TABLE.push_back(CREAT_GLOBAL_TABLE(__value, ast->in_iden.c_str(), ast->in_iden));

  for (auto child : ast->in_body) {
    __Mer_return_Code _code = MerCompiler_compile_ast_id(child, glb);
    INSERT_VEC(for_body, _code);
    _code.prg_code.buff.clear();
  }

  for_loop_iter = MerCompiler_compile_ast_id(ast->in_value, glb);

  INSERT_VEC(result, for_loop_iter);
  PUSH(result, CSTORE_GLOBAL);
  PUSH(result, __iter);

  PUSH(result, CPUSH_FLOAT);
  PUSH(result, 0x00);
  PUSH(result, 0x00);
  PUSH(result, 0x00);
  PUSH(result, 0x00);

  PUSH(result, CSTORE_GLOBAL);
  PUSH(result, __i);

  PUSH(result, CADDRESS);
  PUSH(result, loop_start);

  PUSH(result, CLOAD_GLOBAL);
  PUSH(result, __i);

  PUSH(result, CLOAD_GLOBAL);
  PUSH(result, __iter);

  PUSH(result, CLEN);
  
  PUSH(result, CLESS);

  PUSH(result, CPOP_JUMP_IF_FALSE);
  PUSH(result, loop_end);

  PUSH(result, CLOAD_GLOBAL);
  PUSH(result, __iter);

  PUSH(result, CLOAD_GLOBAL);
  PUSH(result, __i);

  PUSH(result, CGET_ITEM);

  PUSH(result, CSTORE_GLOBAL);
  PUSH(result, __value);

  INSERT_VEC(result, for_body);

  PUSH(result, CLOAD_GLOBAL);
  PUSH(result, __i);

  PUSH(result, CPUSH_FLOAT);
  PUSH(result, 0x00);
  PUSH(result, 0x00);
  PUSH(result, 0x80);
  PUSH(result, 0x3F);

  PUSH(result, CBINARY_ADD);

  PUSH(result, CSTORE_GLOBAL);
  PUSH(result, __i);

  PUSH(result, CJUMP_TO);
  PUSH(result, loop_start);

  PUSH(result, CADDRESS);
  PUSH(result, loop_end);

  GLOBAL_TABLE.erase(GLOBAL_TABLE.begin() + __iter_idx);
  GLOBAL_TABLE.erase(GLOBAL_TABLE.begin() + __i_idx);

  #ifdef SYSTEM_TEST
  cout << "[compiler.cpp] [MerCompiler_compile_ast_for_in_statement] [pass]" << endl;
  #endif
  
  return result;
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_literal(mAST_T *ast, __compiler_u &glb) {
  __Mer_return_Code result = NULL_CODE;
  Mer_uint8_t cval[sizeof(float)];
  if (ast->value >= 1e+20 && !skip) {
    cerr << "System alert: The number is to large, this may cause a crash, do you want to continue?" << endl;
    MER_PAUSE;

    skip = true;
  }

  memcpy(cval, &ast->value, sizeof(float));
  result.prg_code.buff.push_back(CPUSH_FLOAT);
  result.prg_code.buff.insert(result.prg_code.buff.end(), cval, cval + sizeof(float));

  #ifdef SYSTEM_TEST
  cout << "[compiler.cpp] [MerCompiler_compile_ast_literal] [pass]" << endl;
  #endif
  return result;  
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_binary_expression(mAST_T *ast, __compiler_u &glb) {
  __Mer_return_Code _left = MerCompiler_compile_ast_id(ast->left, glb);
  __Mer_return_Code _right = MerCompiler_compile_ast_id(ast->right, glb);

  Mer_uint8_t _oper;
  if (ast->op == PLUS)           _oper = CBINARY_ADD;
  else if (ast->op == MINUS)     _oper = CBINARY_SUB;
  else if (ast->op == DIV)       _oper = CBINARY_DIV;
  else if (ast->op == MOD)       _oper = CBINARY_MOD;
  else if (ast->op == TIME)      _oper = CBINARY_MUL;

  __Mer_return_Code result = NULL_CODE;
  
  INSERT_VEC(result, _left);
  INSERT_VEC(result, _right);
  PUSH(result, _oper);

  #ifdef SYSTEM_TEST
  cout << "[compiler.cpp] [MerCompiler_compile_ast_binary_expression] [pass]" << endl;
  #endif

  return result;
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_function_call(mAST_T *ast, __compiler_u &glb) {
  __Mer_return_Code result = NULL_CODE;

  Mer_uint8_t address;
  Mer_bool found = false;
  Mer_string name = strdup(ast->func_call.c_str());
  for (auto &item : GLOBAL_TABLE) {
    if (item->__name == name) {
      address = item->__Address;
      found = true;
      break;
    }
  }

  if (!found) {
    Mer_real_string str = ast->func_call.c_str();
    Mer_real_string msg = "Function not found " + str;
    MerDebug_system_error(COMPILER_ERROR, msg.c_str(), glb.file);
    return NULL_CODE;
  }

  if (ast->is_having_args) {
    for (auto child : ast->args) {
      __Mer_return_Code _code = MerCompiler_compile_ast_id(child, glb);
      INSERT_VEC(result, _code);
      _code.prg_code.buff.clear();
    }
  }

  PUSH(result, CLOAD_GLOBAL);
  PUSH(result, address);
  PUSH(result, CFUNCTION_CALL);

  #ifdef SYSTEM_TEST
  cout << "[compiler.cpp] [MerCompiler_compile_ast_function_call] [pass]" << endl;
  #endif

  return result;    
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_if(mAST_T *ast, __compiler_u &glb) {
  __Mer_return_Code result = NULL_CODE;
  __Mer_return_Code comp = NULL_CODE;
  __Mer_return_Code if_body = NULL_CODE;
  __Mer_return_Code else_body = NULL_CODE;
  
  comp = MerCompiler_compile_ast_id(ast->comp, glb);
  
  if (!ast->comp) MerDebug_system_error(COMPILER_ERROR, "Condition not found", glb.file);

  ++glb.address;
  Mer_uint8_t end_if = glb.address;
  glb.byte += 0x0002;
  glb.line += 0x0001;

  for (auto child : ast->then_body) {
    __Mer_return_Code _code = MerCompiler_compile_ast_id(child, glb);
    INSERT_VEC(if_body, _code);
    _code.prg_code.buff.clear();
  }

  if (ast->has_else) {
    for (auto child : ast->else_body) {
      __Mer_return_Code _code = MerCompiler_compile_ast_id(child, glb);
      INSERT_VEC(else_body, _code);
      _code.prg_code.buff.clear();
    }

    Mer_uint8_t label_else = ++glb.address;

    INSERT_VEC(result, comp);
    PUSH(result, CPOP_JUMP_IF_FALSE);
    PUSH(result, label_else);
    INSERT_VEC(result, if_body);
    PUSH(result, CJUMP_TO);
    PUSH(result, end_if);
    PUSH(result, CADDRESS);
    PUSH(result, label_else);
    INSERT_VEC(result, else_body);
    PUSH(result, CADDRESS);
    PUSH(result, end_if);

    #ifdef SYSTEM_TEST
    cout << "[compiler.cpp] [MerCompiler_compile_ast_if] [pass]" << endl;
    #endif

    return result;
  }

  INSERT_VEC(result, comp);
  PUSH(result, CPOP_JUMP_IF_FALSE);
  PUSH(result, end_if);
  INSERT_VEC(result, if_body);
  PUSH(result, CADDRESS);
  PUSH(result, end_if);

  #ifdef SYSTEM_TEST
  cout << "[compiler.cpp] [MerCompiler_compile_ast_if] [pass]" << endl;
  #endif
  return result;
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_let(mAST_T *ast, __compiler_u &glb) {
  Mer_string name = strdup(ast->var_name.c_str());
  Mer_real_string real_name = ast->var_name;
  ++glb.address;
  glb.byte += 0x0002;
  glb.line += 0x0001;

  __Mer_return_Code var_val = MerCompiler_compile_ast_id(ast->var_value, glb);
  __Mer_return_Code result = NULL_CODE;

  GLOBAL_TABLE.push_back(CREAT_GLOBAL_TABLE(glb.address, name, real_name));

  INSERT_VEC(result, var_val);
  PUSH(result, CSTORE_GLOBAL);
  PUSH(result, glb.address);

  #ifdef SYSTEM_TEST
  cout << "[compiler.cpp] [MerCompiler_compile_ast_let] [pass]" << endl;
  #endif

  return result;
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_return(mAST_T *ast, __compiler_u &glb) {
  __Mer_return_Code result = NULL_CODE;
  __Mer_return_Code ret_val = NULL_CODE;

  if (ast->is_void) {
    PUSH(result, CRETURN);
    return result;
  }
  
  ret_val = MerCompiler_compile_ast_id(ast->return_v, glb);
  INSERT_VEC(result, ret_val);
  PUSH(result, CRETURN);

  #ifdef SYSTEM_TEST
  cout << "[compiler.cpp] [MerCompiler_compile_ast_return] [pass]" << endl;
  #endif

  return result;
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_identifier(mAST_T *ast, __compiler_u &glb) {
  __Mer_return_Code result = NULL_CODE;
  Mer_uint8_t address;
  Mer_bool found = false;
  Mer_string name = strdup(ast->string_iden.c_str());
  if (name != ast->string_iden) {
    MerDebug_system_error(COMPILER_ERROR, "Invalid identifier", glb.file);
  }

  for (auto &item : GLOBAL_TABLE) {
    if (item->__name == name) {
      address = item->__Address;
      found = true;
      break;
    }
  }

  if (!found) {
    Mer_real_string msg = "Variable not found " + ast->string_iden;
    MerDebug_system_error(COMPILER_ERROR, msg.c_str(), glb.file);
    return NULL_CODE;
  }

  PUSH(result, CLOAD_GLOBAL);
  PUSH(result, address);

  #ifdef SYSTEM_TEST
  cout << "[compiler.cpp] [MerCompiler_compile_ast_identifier] [pass]" << endl;
  #endif
  return result;
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_program(mAST_T *ast, __compiler_u &glb) {
  #ifdef SYSTEM_TEST
  cout << "[compiler.cpp] [MerCompiler_compile_ast_program] [start]" << endl;
  #endif

  #ifdef SYSTEM_TEST
  cout << "[compiler.cpp] [MerCompiler_compile_ast_program] [pass]" << endl;
  #endif

  return MerCompiler_compile_ast_id_expression_statment(ast, glb);
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_id_expression_statment(mAST_T *ast, __compiler_u &glb) {
  
  __Mer_return_Code code = NULL_CODE;
  code.cfile = ast->file;

  mAST_T *root;
  if (ast->children[0]->type == ExpressionStatement) {
    root = ast->children[0];
  }

  PUSH(code, CPROGRAM_START);
  
  for (auto child : root->children) {
    __Mer_return_Code _code = MerCompiler_compile_ast_id(child, glb);
    INSERT_VEC_TO_OUTCODE_AND_PROG_CODE(code, _code);
    _code.prg_code.buff.clear();
  }

  PUSH(code, CPROGRAM_END);

  #ifdef SYSTEM_TEST
  cout << "[compiler.cpp] [MerCompiler_compile_ast_id_expression_statment] [pass]" << endl;
  #endif
  return code;
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_string_var(mAST_T *ast, __compiler_u &glb) {
  __Mer_return_Code result = NULL_CODE;
  Mer_string name = strdup(ast->str_var_name.c_str());
  Mer_real_string real_name = ast->str_var_name;
  glb.address += 0x0001;
  glb.byte += 0x0002;
  glb.line += 0x0001;

  __Mer_return_Code str_val = MerCompiler_compile_ast_id(ast->str_var_value, glb);
  GLOBAL_TABLE.push_back(CREAT_GLOBAL_TABLE(glb.address, name, real_name));

  INSERT_VEC(result, str_val);
  PUSH(result, CSTORE_GLOBAL);
  PUSH(result, glb.address);

  #ifdef SYSTEM_TEST
  cout << "[compiler.cpp] [MerCompiler_compile_ast_string_var] [pass]" << endl;
  #endif

  return result;
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_function(mAST_T *ast, __compiler_u &glb) {
  __Mer_return_Code result = NULL_CODE;
  __Mer_return_Code body = NULL_CODE;
  ++glb.address;
  Mer_uint8_t func_address = glb.address;
  Mer_real_string name = ast->func_name;
  Mer_string func_name = strdup(ast->func_name.c_str());

  glb.byte += 0x0002;
  glb.line += 0x0001;

  for (auto &item : GLOBAL_TABLE) {
    if (item->__name == func_name) {
      Mer_real_string msg = "Function already exist: '" + ast->func_name + "'";
      MerDebug_system_error(COMPILER_ERROR, msg.c_str(), glb.file);
    }
  }
  

  GLOBAL_TABLE.push_back(CREAT_GLOBAL_TABLE(func_address, func_name, name));

  for (auto child : ast->body) {
    if (child->type == ReturnStatement) {
      cout << "return" << endl;
      __Mer_return_Code _code = MerCompiler_compile_ast_id(child, glb);
      INSERT_VEC(body, _code);
      _code.prg_code.buff.clear();
      break;
    }

    __Mer_return_Code _code = MerCompiler_compile_ast_id(child, glb);
    INSERT_VEC(body, _code);
    _code.prg_code.buff.clear();
  }

  PUSH(result, CMAKE_FUNCTION);
  PUSH(result, func_address);
  INSERT_VEC(result, body);
  PUSH(result, CRETURN);

  #ifdef SYSTEM_TEST
  cout << "[compiler.cpp] [MerCompiler_compile_ast_function] [pass]" << endl;
  #endif

  return result;
}

MERCURY_API __mer_core_api__ __Mer_return_Code MerCompiler_compile_ast_comparison_expression(mAST_T *ast, __compiler_u &glb) {
  __Mer_return_Code _left = MerCompiler_compile_ast_id(ast->comp_left, glb);
  __Mer_return_Code _right = MerCompiler_compile_ast_id(ast->comp_right, glb);

  __Mer_return_Code result = NULL_CODE;
  Mer_uint8_t _oper;

  if (ast->comp_op == EQUAL) _oper = CEQUAL;
  else if (ast->comp_op == NOT_EQUAL) _oper = CNOT_EQUAL;
  else if (ast->comp_op == LESS) _oper = CLESS;
  else if (ast->comp_op == LESS_EQUAL) _oper = CLESS_EQUAL;
  else if (ast->comp_op == GREATER) _oper = CGREATER;
  else if (ast->comp_op == GREATER_EQUAL) _oper = CGREATER_EQUAL;
  else if (ast->comp_op == NOT) _oper = CNOT;
  else if (ast->comp_op == AND) _oper = CAND;
  else if (ast->comp_op == OR) _oper = COR;

  INSERT_VEC(result, _left);
  INSERT_VEC(result, _right);
  PUSH(result, _oper);

  #ifdef SYSTEM_TEST
  cout << "[compiler.cpp] [MerCompiler_compile_ast_comparison_expression] [pass]" << endl;
  #endif

  return result;
}
