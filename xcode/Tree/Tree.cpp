//
//  Tree.cpp
//  Orage
//
//  Created by Vincent Evrard on 6/06/18.
//

#include "Tree.hpp"

using namespace std;
//using namespace cinder;
//using namespace cinder::app;
//using namespace glm;

int Tree::ID = 0;

Tree::Tree(){
    this->id = Tree::ID++;
    this->uniqueID = Tree::genUniqueId(this->id);
}


string Tree::genUniqueId(int input){
    ostringstream oss;
    oss << input << std::time(nullptr);
    istringstream iss(oss.str());
    long c;
    iss >> c;
    cout<<c<<endl;
    hashidsxx::Hashids hash(
                            "ORAGE",
                            8,
                            "abcdefghijklmnopqrstuvwxyz"
                            );
    return hash.encode({c});
}

TreeRef Tree::push(TreeRef node){
    node->parent = this;
    node->level = this->level + 1;
    this->nodes[node->uniqueID] = node;
    return node;
}

TreeRef Tree::add(){
    return this->push(new Tree());
}

TreeRef Tree::wrap(vector<TreeRef> tree){
    auto it = tree.begin();
    while(it != tree.end()){
        this->nodes[(*it)->uniqueID] = (*it)->parent->nodes[(*it)->uniqueID];
        auto ot = (*it)->parent->nodes.find((*it)->uniqueID);
        (*it)->parent->nodes.erase(ot);
        (*it)->parent = this;
        (*it)->forSubTree([](TreeRef node){
            node->level = node->parent->level + 1;
        }, true, true);
        it++;
    }
    return this;
}

TreeRef Tree::get(string uniqueID){
    return this->nodes[uniqueID];
}

TreeRef Tree::forSubTree(TreeFnc action, bool recursive, bool inclusive){
    if(inclusive){
        action(this);
    }
    auto it = this->nodes.begin();
    while(it != this->nodes.end()){
        if(!recursive || !inclusive){
            action(it->second);
        }
        if(recursive){
            it->second->forSubTree(action, recursive, inclusive);
        }
        it++;
    }
    return this;
}

TreeRef Tree::forUpTree(TreeFnc action){
    action(this);
    if(this->parent) {
        this->parent->forUpTree(action);
    }
    return this;
}

void Tree::printTree(){
    this->forSubTree([](TreeRef node){
        for(int i = 0 ; i < node->level ; i ++){
            cout<<"\t";
        }
        cout<<node->id<<" : "<<node->uniqueID<<endl;
    }, true, true);
}

vector<TreeRef> Tree::getNodes(){
    vector<TreeRef> result;
    this->forSubTree([&](TreeRef node){
        result.push_back(node);
    });
    return result;
}

