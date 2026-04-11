char* strcatf( char* dest, char* src ) // makes this shi like 5 times faster or more cus C be smart
{
    while (*dest) dest++;
    while (*dest++ = *src++);
    return --dest;
}