//
//  Tree.hpp
//  Orage
//
//  Created by Vincent Evrard on 6/06/18.
//

#pragma once
#include "hashids.h"

class Tree {
    static int ID;
    static std::string genUniqueId(int input);
    public :
        typedef Tree * TreeRef;
        typedef std::function<void(TreeRef)> TreeFnc;
        Tree();
        virtual ~Tree(){}
        std::string uniqueID;
        std::map <std::string, TreeRef> nodes;
        int id ;
        int level = 0;
        TreeRef parent;
        TreeRef push(TreeRef node);
        /*
         * create a child node for this
         * return the child
         */
        virtual TreeRef add();
        TreeRef wrap(std::vector<TreeRef> tree);
        TreeRef get(std::string uniqueID);
        /*
         * perform an action on this and on first order node children of this
         * if recursive is true
         * perform recursively an action on this and and all orders children of this
         * if inclusive is true
         * perform the action even on this
         */
        TreeRef forSubTree(TreeFnc action, bool recursive = false, bool inclusive = false);
        /*
         * perform recursive an action on this and on parent of this
         */
        TreeRef forUpTree(TreeFnc action);
        void printTree();
        std::vector<TreeRef> getNodes();
};
typedef Tree * TreeRef;
