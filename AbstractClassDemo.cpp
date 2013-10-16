#include <stdio.h>
/*
reference
http://stackoverflow.com/questions/650091/using-an-abstract-class-in-c
Robert S. Barnes
*/

class A {
    public:
        virtual void action() = 0;
};

class B : public A {
    public:
        B() {}

        void action() {
                printf("Hello World\n");
        }
};

class C : public A {
    public:
        C() {}

        void action() {
                printf("Goodbye World\n");
        }
};

class AHandleClass {

    public:

        void setInstance(A *A_Instance) {
                APointer = A_Instance;
        }

        void doSomething() {
                APointer->action();
        }

    private:

        A *APointer;
};

int main(int argc, char** argv) {
    AHandleClass AHandle;
    B BInstance;
    C CInstance;
    AHandle.setInstance(&BInstance);
    AHandle.doSomething();
    AHandle.setInstance(&CInstance);
    AHandle.doSomething();
    return 0;
}
