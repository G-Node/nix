
#include <pandora/Block.hpp>

using namespace std;

namespace pandora {


/* SEE: SimpleTag.hpp */
Block::Block(const Block &block) :
    NamedEntityWithSources(block.file, block.group, block.entity_id)
{}

/* SEE: SimpleTag.hpp */
Block::Block(File *file, Group group, std::string id) :
    NamedEntityWithSources(file, group, id)
{}

Block::~Block() {
  //dtor
}

} // end namespace pandora
