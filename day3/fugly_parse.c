/* part 1 */
static int parse_input(char *line)
{
    int a, b;
    int result = 0;
    
    for (char *current = strstr(line, "mul"); current; current = strstr(current, "mul")) {
        char open_bracket = 0;
        char close_bracket = 0;
        char comma = 0;

        if (sscanf(current, "mul%c%d%c%d%c", &open_bracket, &a, &comma, &b, &close_bracket) == 5) {
            if(open_bracket == '(' && comma == ',' && close_bracket == ')') {
                result += multiply(a, b);
            }
        }

        current++;
    }
    return result;
}

/* part 2 bad and ugly */
static int conditioned_parse_input(char *line)
{
    char *current = line;
    char *dont_cond_pos = NULL;
    char *do_cond_pos = NULL;
    char *mul_pos = NULL;
    int a, b;
    int result = 0;
    int condition = DO;
    
    for (char *current = line; current; current++) {
        char open_bracket = 0;
        char close_bracket = 0;
        char comma = 0;

        mul_pos = strstr(current, "mul(");
        do_cond_pos = strstr(current, "do()");
        dont_cond_pos = strstr(current, "don't()");

        if (!mul_pos) {
            break;
        }

        if (do_cond_pos && do_cond_pos < mul_pos) {
            condition = DO;
        }

        if (dont_cond_pos && dont_cond_pos < mul_pos) {
            condition = DO_NOT;
        }

        if (sscanf(mul_pos, "mul%c%d%c%d%c", &open_bracket, &a, &comma, &b, &close_bracket) == 5 &&
            open_bracket == '(' && comma == ',' && close_bracket == ')') {
            if (condition) {
                result += multiply(a, b);
            }
            current += mul_pos - current;
        }
    }
    return result;
}

/* part 2 just ugly */
static int conditioned_parse_input(char *line)
{
    char *last_processed = line;
    char *processing = NULL;
    char *dont_pos = NULL;
    char *do_pos = NULL;
    int a, b;
    int result = 0;
    int condition = DO;
    
    for (processing = strstr(line, "mul("); processing; processing = strstr(last_processed, "mul(")) {
        char open_bracket = 0;
        char close_bracket = 0;
        char comma = 0;
        
        do_pos = strstr(last_processed, "do()");
        dont_pos = strstr(last_processed, "don't()");

        if (do_pos && do_pos < processing) {
            condition = DO;
        }

        if (dont_pos && dont_pos < processing) {
            condition = DO_NOT;
        }

        if (sscanf(processing, "mul%c%d%c%d%c", &open_bracket, &a, &comma, &b, &close_bracket) == 5 &&
            open_bracket == '(' && comma == ',' && close_bracket == ')') {
            if (condition) {
                result += multiply(a, b);
            }
            last_processed += processing - last_processed;
        }
        last_processed++;
    }
    return result;
}