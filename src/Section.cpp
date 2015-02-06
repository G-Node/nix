// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/Section.hpp>

#include <list>
#include <algorithm>
#include <iterator>


using namespace std;
using namespace nix;

Section::Section()
    : NamedEntity()
{
}


Section::Section(nullptr_t ptr)
    : NamedEntity()
{
}


Section::Section(const Section &other)
    : NamedEntity(other.impl())
{
}


Section::Section(const shared_ptr<base::ISection> &p_impl)
    : NamedEntity(p_impl)
{
}


Section::Section(shared_ptr<base::ISection> &&ptr)
    : NamedEntity(std::move(ptr))
{
}


void Section::link(const Section &link) {
    if (link == none) {
        backend()->link(none);
    } else {
        backend()->link(link.id());
    }
}

//-----------------------------------------------------
// Methods concerning child sections
//-----------------------------------------------------


bool Section::hasSection(const Section &section) const {
    if (section == none) {
        throw std::runtime_error("Section::hasSection: Empty Section entity given!");
    }
    return backend()->hasSection(section.id());
}


bool Section::deleteSection(const Section &section) {
    if (section == none) {
        throw std::runtime_error("Section::deleteSection: Empty Section entity given!");
    }
    return backend()->deleteSection(section.id());
}

/*
 * Helper struct for {@link findSections}.
 */
struct SectionCont {
    SectionCont(Section s, size_t d = 0)
    : entity(s), depth(d)
    {}

    Section entity;
    size_t depth;
};


std::vector<Section> Section::sections(const util::Filter<Section>::type &filter) const {
    auto f = [this] (size_t i) { return getSection(i); };
    return getEntities<Section>(f,
                                sectionCount(),
                                filter);
}


std::vector<Section> Section::findSections(const util::Filter<Section>::type &filter,
                                           size_t max_depth) const
{
    std::vector<Section>  results;
    std::list<SectionCont> todo;

    todo.push_back(SectionCont(*this));

    while (todo.size() > 0)
    {
        SectionCont current = todo.front();
        todo.pop_front();

        bool filter_ok = filter(current.entity);
        if (filter_ok) {
            results.push_back(current.entity);
        }

        if (current.depth < max_depth) {
            std::vector<Section> children = current.entity.sections();
            size_t next_depth = current.depth + 1;

            for (auto it = children.begin(); it != children.end(); ++it) {
                todo.push_back(SectionCont(*it, next_depth));
            }
        }
    }

    return results;
}

static inline auto erase_section_with_id(vector<Section> &sections, const string &my_id)
    -> decltype(sections.size())
{
    sections.erase(remove_if(sections.begin(),
                             sections.end(),
                             [&my_id](const Section &section) {
                                 return my_id == section.id();
                             }),
                   sections.end());

    return sections.size();
}

std::vector<Section> Section::findRelated(const util::Filter<Section>::type &filter) const
{
    std::vector<Section> results = findDownstream(filter);
    const string &my_id = id();

    //This checking of results can be removed if we decide not to include this in findSection
    auto results_size = erase_section_with_id(results, my_id);

    if (results_size == 0) {
        results = findUpstream(filter);
    }
    //This checking of results can be removed if we decide not to include this in findSection
    results_size = erase_section_with_id(results, my_id);

    if (results_size == 0) {
        results = findSideways(filter, id());
    }
    return results;
}


//-----------------------------------------------------
// Methods for property access
//-----------------------------------------------------

bool Section::hasProperty(const Property &property) const {
    if (property == none) {
        throw std::runtime_error("Section::hasProperty: Empty Property entity given!");
    }
    return backend()->hasProperty(property.id());
}

std::vector<Property> Section::properties(const util::Filter<Property>::type &filter) const {
    auto f = [this] (size_t i) { return getProperty(i); };
    return getEntities<Property>(f,
            propertyCount(),
            filter);
}

bool Section::deleteProperty(const Property &property) {
    if (property == none) {
        throw std::runtime_error("Section::deleteProperty: Empty Property entity given!");
    }
    return backend()->deleteProperty(property.id());
}

vector<Property> Section::inheritedProperties() const {

    vector<Property> own = properties();

    if (link() == nullptr)
        return own;

    const vector<Property> linked = link().properties();

    copy_if (linked.begin(), linked.end(),
             back_inserter(own),
             [&own](const Property &linked_prop) {
                 return find_if (own.begin(), own.end(),
                                [&linked_prop](const Property &own_prop) {
                                    return linked_prop.name() == own_prop.name();
                                }) == own.end();
             });

    return own;
}


//------------------------------------------------------
// Operators and other functions
//------------------------------------------------------

size_t Section::tree_depth() const{
  const vector<Section> children = sections();
  size_t depth = 0;
  if (children.size() > 0) {
      for (auto &child : children) {
          depth = max(depth, child.tree_depth());
      }
      depth += 1;
  }
  return depth;
}


vector<Section> Section::findDownstream(const std::function<bool(Section)> &filter) const{
    vector<Section> results;
    size_t max_depth = tree_depth();
    size_t actual_depth = 1;
    while (results.size() == 0 && actual_depth <= max_depth) {
        results = findSections(filter, actual_depth);
        actual_depth += 1;
    }
    return results;
}


vector<Section> Section::findUpstream(const std::function<bool(Section)> &filter) const{
    vector<Section> results;
    Section p = parent();

    if (p != nullptr) {
        results = p.findSections(filter,1);
        if (results.size() > 0) {
            return results;
        }
        return p.findUpstream(filter);
    }
    return results;
}


vector<Section> Section::findSideways(const std::function<bool(Section)> &filter, const string &caller_id) const{
    vector<Section> results;
    Section p = parent();
    if (p != nullptr) {
        results = p.findSections(filter,1);
        if (results.size() > 0) {

            results.erase(remove_if(results.begin(),
                                    results.end(),
                                    [&caller_id](const Section &section) {
                                        return section.id() == caller_id;
                                    }),
                          results.end());

            return results;
        }
        return p.findSideways(filter, caller_id);
    }
    return results;
}


std::ostream& nix::operator<<(ostream &out, const Section &ent) {
    out << "Section: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}
