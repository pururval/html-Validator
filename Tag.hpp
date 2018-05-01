/**
 * Tag.hpp
 * 
 * This contains the code for the Tag class.
 * Do not modify any of the contents here.
 */

#ifndef A4_TAG_HPP
#define A4_TAG_HPP

#include <string>
#include <vector>


class Tag {
public:
    enum TAGNAME {
        HTML, HEAD, BODY, TITLE, DIV, P, BR, SPAN, CONTENT
    };

    std::vector<Tag*> _children;
    const std::string _name;
    const std::string _id;
    std::string _content;
    const TAGNAME _tagname;
    bool _displayed;

    // Must create tags with the tag name and ID upfront.
    Tag(const std::string& name, const std::string& id = "") : _name(name), _id(id), _content(""), _tagname(TAGNAME::CONTENT), _displayed(false) {}
    Tag(const std::string& name, const TAGNAME& tagname, const std::string& id = "") : _name(name), _id(id), _content(""), _tagname(tagname), _displayed(false) {}
};


#endif //A4_TAG_HPP
