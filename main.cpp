#include <iostream>
#include <pandora.hpp>

using namespace pandora;
using namespace std;

int main(int args, char **argv)
{
  File *f1 = new File("test_block.h5", "org.g-node");
  const char *names[5] = {"block_a", "block_b", "block_c", "block_d", "block_e"};
  for (int i = 0; i < 5; i++) {
    Block b1(f1->createBlock(names[i], "dataset"));
    cout << "ID b1: " << b1.id();
    Block b2(f1->getBlock(b1.id()));
    cout << " / ID b2: " << b2.id() << " / b1 == b2 = " << (b1 == b2) << endl;
  }
  delete f1;
}
