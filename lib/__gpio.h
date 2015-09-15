struct GPIO_VALUES {
    int SoC_number;
    int Board_pin_number;
    char * Board_text_name;
    char * SoC_text_name;
};

static struct GPIO_VALUES * current_board;

/*    {   4, 28, "GPIO_F", "GPIO_4", "  4" },*/
static struct GPIO_VALUES dragon[] = {
    {  36, 23, "GPIO_A", "GPIO_36" },
    {  12, 24, "GPIO_B", "GPIO_12" },
    {  13, 25, "GPIO_C", "GPIO_13" },
    {  69, 26, "GPIO_D", "GPIO_69" },
    { 115, 27, "GPIO_E", "GPIO_115" },
    { 507, 28, "GPIO_F", "GPIO_4" },
    {  24, 29, "GPIO_G", "GPIO_24" },
    {  25, 30, "GPIO_H", "GPIO_25" },
    {  35, 31, "GPIO_I", "GPIO_35" },
    {  34, 32, "GPIO_J", "GPIO_34" },
    {  28, 33, "GPIO_K", "GPIO_28" },
    {  33, 34, "GPIO_L", "GPIO_33" },
};

static struct GPIO_VALUES hikey[] = {
    {  488, 23, "GPIO_A", "GPIO2_0" },
    {  489, 24, "GPIO_B", "GPIO2_1" },
    {  490, 25, "GPIO_C", "GPIO2_2" },
    {  491, 26, "GPIO_D", "GPIO3_3" },
    {  492, 27, "GPIO_E", "GPIO2_4" },
    {  415, 28, "GPIO_F", "BL_PWDN0GPIO9_1" },
    {  463, 29, "GPIO_G", "GPIO6_7_DSI_TE0" },
    {  495, 30, "GPIO_H", "GPIO2_7" },
    {  426, 31, "GPIO_I", "ISP_RSTB0_GPIO10_2" },
    {  433, 32, "GPIO_J", "ISP_RSTB0_GPIO9_1" },
    {  427, 33, "GPIO_K", "GSP_RSTB1_GPIO10_37" },
    {  434, 34, "GPIO_L", "ISP_RSTB0_GPIO9_2" },
};

static struct GPIO_VALUES bubblegum[] = {
    { 19, 23, "GPIO_A", "GPIOD19" },
    { 18, 24, "GPIO_B", "GPIOD18" },
    { 17, 25, "GPIO_C", "GPIOD17" },
    { 16, 26, "GPIO_D", "GPIOD16" },
    { 15, 27, "GPIO_E", "GPIOD15" },
    { 14, 28, "GPIO_F", "GPIOD14" },
    { 13, 29, "GPIO_G", "GPIOD13" },
    { 12, 30, "GPIO_H", "GPIOD12" },
    { 11, 31, "GPIO_I", "GPIOD11" },
    { 10, 32, "GPIO_J", "GPIOD10" },
    { 27, 33, "GPIO_K", "GPIOD27" },
    { 26, 34, "GPIO_L", "GPIOD26" },
};

