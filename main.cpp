#include <iostream>
#include <pandora.hpp>

using namespace pandora;
using namespace std;

int main(int args, char **argv)
{
  File *f1 = new File("test_block.h5", "org.g-node");

  for(BlockIterator i(f1->blocks()); i != i.end(); ++i) {
    Block b(*i);
    cout << b.name() << endl;
  }

  delete f1;
}
