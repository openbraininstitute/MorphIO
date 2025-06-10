macro(morphio_variable_push var_name)
  list(APPEND morphio_${var_name}_old "${${var_name}}")
endmacro()

macro(morphio_variable_pop var_name)
  list(POP_BACK morphio_${var_name}_old "${var_name}")
endmacro()
