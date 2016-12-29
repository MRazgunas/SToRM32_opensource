This is firmware designed to run on SToRM32 board. This is not same FW as original SToRM32. For now
<h1>DO NOT USE THIS FIRMWARE</h1>

<h1>Adding new parameters</h1>
<ol>
<li>Add new parameter to EEPROM layout enum (located in parameters_d.h) using this format: k_param_xx (DO NOT CHANGE EXISTING ORDER!!!)</li>
<li>Define new variable using xx name in parameters_d.h as extern and in parameters_d.c (supported types int8_t, int16_t, int32_t and float)</li>
<li>Add new parameter to var_list array located in parameters_d.c using this format: GSCALAR(param_type, xx, "PARAM_NAME", def_value)</li>
</ol>
