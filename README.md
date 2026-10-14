# C++ Virtual Method Tables and Object Memory Layout Demo 

This repository is for educational purposes only. It contains a small C++ experiment exploring how polymorphic objects are commonly represented in memory and how virtual method dispatch can be affected by changing the virtual-table pointer.

This is an implementation-specific (x86_64), intentionally unsafe demo. It relies on assumptions about compiler-generated object layout and virtual-table representation, and several operations invoke undefined or implementation-defined behavior. It is intended for learning and experimentation only code.


## Generated output (gcc/Linux x86_64)

I'm Base
Base::print(), val == 0x01010101

Binary representation of Base object:
58 cd 4d a8 7a 60 00 00 
01 01 01 01 3a 00 00 00 

VMT table: Base
  0: 0x607aa84da78c
  1: 0x607aa84da7bc

Base::print(), val == 0x01010101
--------

I'm Derived
Derived::print(), val == 0x22222222, l == 202020202020202

Binary representation of Derived object:
38 cd 4d a8 7a 60 00 00 
22 22 22 22 00 00 00 00 
02 02 02 02 02 02 02 02 

VMT table: Derived
  0: 0x607aa84da86c
  1: 0x607aa84da8a4

+++++++++


Binary representation of the class emulator structure:
20 d0 4d a8 7a 60 00 00 
33 33 33 33 00 00 00 00 
33 33 33 33 33 33 33 33 

VMT table: classEmulator
  0: 0x607aa84da86c
  1: 0x607aa84da8a4

I'm Derived
Derived::print(), val == 0x33333333, l == 3333333333333333

!!!!!!!!!


Binary representation of the class emulator structure:
20 d0 4d a8 7a 60 00 00 
33 33 33 33 00 00 00 00 
33 33 33 33 33 33 33 33 

VMT table: classEmulator
  0: 0x607aa84da78c
  1: 0x607aa84da8a4

I'm Derived
Base::print(), val == 0x33333333

&&&&&&&&&&&&&

I'm Derived

I'm just some function called with an argument pointed to this data
20 d0 4d a8 7a 60 00 00 
33 33 33 33 00 00 00 00 
33 33 33 33 33 33 33 33 

@@@@@@@@@@@

I'm Derived
Derived::print(), val == 0x44444444, l == 404040404040404

Binary representation of the Derived object in heap:
38 cd 4d a8 7a 60 00 00 
44 44 44 44 00 00 00 00 
04 04 04 04 04 04 04 04 

Oups... memory has just been overwritten


Binary representation of the updated Derived object in heap:
20 d0 4d a8 7a 60 00 00 
44 44 44 44 00 00 00 00 
04 04 04 04 04 04 04 04 
I'm Derived

I'm just some function called with an argument pointed to this data
20 d0 4d a8 7a 60 00 00 
44 44 44 44 00 00 00 00 
04 04 04 04 04 04 04 04 




