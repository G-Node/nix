#include <iostream>
#include <pandora.hpp>

using namespace pandora;
using namespace std;

int main(int args, char **argv)
{
  File *f1 = new File("test_block.h5", "org.g-node");
  File *f2 = new File("test_block.h5", "org.g-node");

  Group g1;
  H5::Group g2;

//  cout << "f1.data id: " << f1->data.h5group.getId() << endl;
//  cout << "f1.data loc_id: " << f1->data.h5group.getLocId() << endl;
//  g1 = f1->data;
//  cout << "g1 id: " << g1.h5group.getId() << endl;
//  cout << "g1 loc_id: " << g1.h5group.getLocId() << endl;
//  g2 = f1->data.h5group;
//  cout << "g2 id: " << g2.getId() << endl;
//  cout << "g2 loc_id: " << g2.getLocId() << endl << endl;
//
//  cout << "f2.data id: " << f2->data.h5group.getId() << endl;
//  cout << "f2.data loc_id: " << f2->data.h5group.getLocId() << endl;
//  g1 = f2->data;
//  cout << "g1 id: " << g1.h5group.getId() << endl;
//  cout << "g1 loc_id: " << g1.h5group.getLocId() << endl;
//  g2 = f2->data.h5group;
//  cout << "g2 id: " << g2.getId() << endl;
//  cout << "g2 loc_id: " << g2.getLocId() << endl << endl;

  delete f1;
  delete f2;
}
