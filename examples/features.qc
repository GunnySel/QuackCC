struct Data2
{
    char type;
    unsigned int length;
} packed align(16);

#define NULL (void*)(0)

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
    // turns into
    Data value; memset(&value, 0, sizeof(Data));

    printf("%d", add(10));
    printf("%d", add(10, 5));

    for (int i = 0; i < length; i++)
    {// must be here, or error

    }

    if (1)
    {// must be here, or error
                
    }

    // Error unless default: is provided:
    switch (v)
    {
        case 1: fallthrough
        case 2:
        {
            
        }
    }
/*
int add(int a, int b=1)
{
    return a + b;
}
int mul(int a, int b=1)
{
    return a * b;
}
*/
    // In C
    funcptr int op_func(int, int) = mul;

    unsigned long size = sizeof(Data, length);
    unsigned long offset = offsetof(Data, length);
} 
