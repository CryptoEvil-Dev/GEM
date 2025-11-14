#include "include/Node.hpp"

GEM::Node::Node(){
    this->tree.clear();
    this->style = nullptr;
}

GEM::Node::Node(std::string content){
    this->tree.clear();
    this->style = nullptr;
    this->content = content;
}

GEM::Node::Node(std::string content, Style style){
    this->tree.clear();
    this->style = std::make_unique<Style>(style);
    this->content = content;
}

GEM::Node::Node(const Node& node) noexcept{
    this->tree = node.tree;
    if(node.style)
        this->style = std::make_unique<Style>(*node.style);
    this->content = node.content;
}

GEM::Node::Node(Node&& node) noexcept{
    this->tree = std::move(node.tree);
    this->style = std::move(node.style);
    this->content = std::move(node.content);
}

GEM::Node& GEM::Node::operator=(const GEM::Node&& other) noexcept {
    if(this != &other){
        this->tree = std::move(other.tree);
        this->style = std::make_unique<Style>(*other.style);
        this->content = std::move(other.content);
    }
    return *this;
}

GEM::Node& GEM::Node::operator=(const GEM::Node& other) noexcept {
    if(this != &other){
        tree = other.tree;
        if(other.style)
            style = std::make_unique<Style>(*other.style);
        else
            style.reset();
        content = other.content;
    }
    return *this;
}


std::vector<GEM::Node>& GEM::Node::GetChilds(){
    return tree;
}

GEM::Node* GEM::Node::GetChild(size_t index){
    return &tree[index];
}

void GEM::Node::RemoveChild(size_t index){
    if(index < tree.size()){
        tree.erase(tree.begin() + index);
    }
}

void GEM::Node::SetContent(const std::string& content){
    this->content = content;
}

const std::string& GEM::Node::GetContent() const {
    return this->content;
}