#include <cstdio>

using namespace std;

class Base {
protected:
    int val;
public:
    Base(int v): val( v ) { }

    Base(): val( 0x01010101 ) {}

    virtual void print() { printf("Base::print(), val == 0x%08x\n", val); }
    virtual void whoAmI() { printf("I'm Base\n" ); }

    Base(Base& other) = delete;
    Base(Base&& other) = delete;
};

class Derived : public Base {
protected:
    long l;
public:
    Derived(int v, long lv) : Base(v), l( lv ) {}
    Derived(int v) : Base(v), l(0x0202020202020202) {}
    Derived() : Base(), l(0x0202020202020202) {}

    virtual void print() { printf("Derived::print(), val == 0x%08x, l == %lx\n", val, l); }
    virtual void whoAmI() { printf("I'm Derived\n" ); }

    Derived(Derived& other) = delete;
    Derived(Derived&& other) = delete;
};

void printByteDump(void *ptr, size_t size, char *msg) {
    unsigned char *p = (unsigned char *)ptr;

    printf("\n%s\n", msg);

    while (size-- > 0) {
        printf("%02x ", *p++);
        if ( ! (size % 8)  ) printf("\n");
    }

}

void printVMT(void ***ptr, size_t size, char *msg) {

    void** vtable = *ptr;

    printf("\nVMT table: %s\n", msg);
    for (size_t i = 0; i < size; i++)
        printf("%3zu: %p\n", i, vtable[i] );
    printf("\n");
}

void justSomeFunc(void *ptr) {
    printByteDump( ptr, 3 * 8, "I'm just some function called with an argument pointed to this data" );
}

void * ownVMT[ 2 ] = { 0, 0 };

struct classEmulator {
    void *ptr = ownVMT;
    long l1, l2;
} ca;



int main() {
    Base b;

    b.whoAmI();
    b.print();

    printByteDump(&b, sizeof(b), "Binary representation of Base object:");
    printVMT( (void ***)&b, 2, "Base");

    void *addrMethodOfBase = (*((void ***)&b ))[0];
    typedef void (*PrintFunc)(Base*);
    // PrintFunc func = reinterpret_cast<PrintFunc>(addrMethodOfBase);
    PrintFunc func = (PrintFunc)addrMethodOfBase;

    func(&b);

    printf("--------\n\n");

    Derived d( 0x22222222 );

    d.whoAmI();
    d.print();
    printByteDump(&d, sizeof(d), "Binary representation of Derived object:");
    printVMT( (void ***)&d, 2, "Derived");

    printf("+++++++++\n\n");

    ownVMT[ 0 ] = (*((void ***)&d ))[ 0 ];
    ownVMT[ 1 ] = (*((void ***)&d ))[ 1 ];
    ca.l1 = 0x33333333;
    ca.l2 = 0x3333333333333333ull;

    printByteDump(&ca, sizeof(ca), "Binary representation of the class emulator structure:");
    printVMT( (void ***)&ca, 2, "classEmulator");

    ((Derived *) &ca) -> whoAmI();
    ((Derived *) &ca) -> print();

    printf("\n!!!!!!!!!\n\n");

    ownVMT[ 0 ] = (*((void ***)&b ))[ 0 ];

    printByteDump(&ca, sizeof(ca), "Binary representation of the class emulator structure:");
    printVMT( (void ***)&ca, 2, "classEmulator");

    ((Derived *) &ca) -> whoAmI();
    ((Derived *) &ca) -> print();

    printf("\n&&&&&&&&&&&&&\n\n");

    ownVMT[ 0 ] = (void *)justSomeFunc;

    Derived* der = (Derived*)&ca;

    der->whoAmI();
    der->print();

    printf("\n@@@@@@@@@@@\n\n");

    der = new Derived( 0x44444444, 0x0404040404040404ll );

    der->whoAmI();
    der->print();

   printByteDump( der, sizeof(*der), "Binary representation of the Derived object in heap:");

    (*((void **)der )) = ownVMT;

    printf("\nOups... memory has just been overwritten\n\n");

    printByteDump( der, sizeof(*der), "Binary representation of the updated Derived object in heap:");

    der->whoAmI();
    der->print();

    delete der;

    return 0;
}
