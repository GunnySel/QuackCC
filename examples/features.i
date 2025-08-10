int b = 3;
extern void* malloc(unsigned long size);
extern void free(void* ptr);
extern long strlen(const char* str);
extern int printf(const char* format, ...);
extern void* memcpy(void *dest, const void *src, unsigned long n);

struct Data2
{
    char type;
    unsigned int length;
} packed align(16);
struct Data
{
    Data2 v;
    char type;
    unsigned int length;
} packed align(16);
int add(int a, int b=1)
{
    return a + b;
}
int mul(int a, int b=1)
{
    return a * b;
}
enum Type : unsigned char
{
    Login = 0,
    Signup = 1,
    Logout = 2,
    CreateRoom = 3,
    JoinRoom = 4,
}
static void print()
{
    printf("Quack!\n");
}
int main()
{
    print();
    Type b = Type::Login;
    char a = 3;
    printf("%d", a);
    Data value = zero;
    Data value = (void*)(0);
    Data value; memset(&value, 0, sizeof(Data));
    printf("%d", add(10));
    printf("%d", add(10, 5));
    for (int i = 0; i < length; i++)
    {
    }
    if (1)
    {
    }
    switch (v)
    {
        case 1: fallthrough
        case 2:
        {
        }
    }
    funcptr int op_func(int, int) = mul;
    unsigned long size = sizeof(Data, length);
    unsigned long offset = offsetof(Data, length);
}
