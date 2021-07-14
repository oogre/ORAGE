//
//  Tree.h
//  Orage
//
//  Created by Vincent Evrard on 1/07/21.
//

#ifndef Tree_h
#define Tree_h

namespace ORAGE {
    namespace COMMON {
        
        using namespace std;
        
        class Node : public enable_shared_from_this<Node> {
            static int ID;
            typedef shared_ptr<class Node> NodeRef;
            typedef function<void(NodeRef)> NodeCallback;
            Node * parent = nullptr;
            
            string name;
            string type;
        protected:
            Node(std::string name, string type = "node") :
                name(name),
                type(type)
            {
                id = Node::ID++;
            }
            vector<NodeRef> nodes;
        public:
            int id ;
            static NodeRef create(string name){
                return NodeRef( new Node(name) );
            }
            virtual ~Node(){
                
            }
            virtual NodeRef addNode(NodeRef node){
                if(_::any(nodes, [&](NodeRef n) { return n->name == node->name; })){
                    throw invalid_argument("addNode : that name already used at this lvl : " + node->name);
                }
                node->setParent(this);
                nodes.push_back(node);
                return node;
            }
            virtual NodeRef getNode(string _name){
                string name ("");
                string rest ("");
                Tools::split(_name, '-', &name, &rest);
                auto temp = _::find(nodes, [&](NodeRef node){
                    return node->name == name;
                });
                if(temp != nodes.end()){
                    if(rest.length()==0){
                        return *temp;
                    }else{
                        return (*temp)->getNode(rest);
                    }
                }
                return nullptr;
            }
            template<typename T = Node>
            vector<NodeRef> getNodes(vector<NodeRef> memo = vector<NodeRef>()){
                for(auto node : nodes){
                    vector<NodeRef> sub = node->getNodes<T>(memo);
                    sub = _::difference<vector<NodeRef>>(sub, memo);
                    memo.insert( memo.end(), sub.begin(), sub.end() );
                }
                if(is<T>()){
                    memo.push_back(as());
                }
                return memo;
            }
            
            template<typename T = Node>
            void every(function<void(shared_ptr<T>)> action){
                action(as<T>());
                for(auto node : nodes){
                    node->every<T>(action);
                }
            }
            template<typename T = Node>
            shared_ptr<T> getParent(function<bool(shared_ptr<T>)> action){
                if(!hasParent()){
                    return nullptr;
                }
                if(getParent()->is<T>() && action(getParent<T>())){
                    return getParent<T>();
                }else{
                    return getParent()->getParent(action);
                }
            }
            
            template<typename T = Node>
            bool is(){
                return as<T>() != NULL;
            }
            template<typename T = Node>
            shared_ptr<T> as(){
                return dynamic_pointer_cast<T>(shared_from_this());
            }
            virtual void setParent(Node * parent){
                this->parent = parent;
            }
            virtual bool hasParent(){
                return !(parent == nullptr || parent == NULL);
            }
            template<typename T = Node>
            shared_ptr<T> getParent(bool recursive = false){
                if(recursive && hasParent()){
                    return parent->getParent<T>(recursive);
                }
                if(hasParent()){
                    return parent->as<T>();
                }
                return as<T>();
            }
            int getDepth(){
                if(hasParent()) return parent->getDepth() + 1;
                return 0;
            }
            
            virtual string getName(bool recursive = false){
                string n = name;
                if(recursive && hasParent()){
                    n = parent->getName(recursive) + "-" + n;
                }
                return n;
            }
            virtual void setName(string name){
                this->name = name;
            };
            void forEach(NodeCallback callback, bool recursive = false){
                for(auto& node : nodes){
                    callback(node);
                    if(recursive){
                        node->forEach(callback);
                    }
                }
            }
            void forEachToRoot(NodeCallback callback){
                callback(as());
                if(hasParent()){
                    parent->forEachToRoot(callback);
                }
            }
            virtual string to_string(){
                string result = getName()+"("+type+")";
                forEach([&](NodeRef node) -> void {
                    result = result + "\n";
                    for(int i = 0 ; i < node->getDepth() ; i ++){
                        result = result + "\t";
                    }
                    result = result + node->to_string();
                });
                
                return result;
            }
        };//class Node
        typedef shared_ptr<class Node> NodeRef;
        int Node::ID = 0;
    }//namespace COMMON
}//namespace ORAGE
#endif /* Tree_h */
