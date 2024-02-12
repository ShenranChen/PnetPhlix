

#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <vector>
#include <iostream>

template <typename KeyType, typename ValueType>
class TreeMultimap
{
private:
    struct Node;
  public:
    class Iterator
    {
      public:
        Iterator()
        {
            m_valid = false;
            vectorNode = nullptr;
            index = 0;
        }
        Iterator( Node* head, KeyType k)
        {
            Node* cur = head;
            m_valid = false;
            vectorNode = nullptr;
            index = 0;
            //use the tree to find where to point the node
            while (cur != nullptr)
            {
                if(k == cur->key)
                {
                    vectorNode = cur;
                    if(vectorNode -> values.size() > 0)
                    {
                        m_valid = true;
                    }
                    break;
                }
                else if( k < cur->key)
                {
                    cur = cur->left;
                }
                else if( k > cur->key)
                {
                    cur = cur-> right;
                }
            }
        }

        ValueType& get_value() const
        {
            if(is_valid())
            {
                return vectorNode->values[index];
            }
            throw 1;
        }

        bool is_valid() const
        {
            return m_valid;
        }

        void advance()
        {
            if(index+1 >= vectorNode->values.size())
            {
                m_valid = false;
            }
            if(is_valid())
            {
                index++;
            }
//            else
//            {
//                std::cerr << "oops, invalid iterator" << std::endl;
//            }
        }

      private:
       bool m_valid;
        Node* vectorNode;
        int index;
    };

    TreeMultimap()
    {
        head = nullptr;
    }

    ~TreeMultimap()
    {
        treeDies(head);
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        
        if(head == nullptr)
        {
            Node* n = new Node(key, value);
            head = n;
        }
        else
        {
            Node* cur = head;
            for(;;)
            {
                //alreadya node, add it to the vector in the node
                if(key == cur->key)
                {
                    cur->values.push_back(value);
                    return;
                }
                if(key < cur->key)
                {
                    //keep going left
                    if(cur->left != nullptr)
                    {
                        cur = cur->left;
                    }
                    else
                    {   //no node yet, create one
                        Node* n = new Node(key, value);
                        cur->left = n;
                        return;
                    }
                }
                else if( key > cur->key)
                {
                    //keep going right
                    if(cur->right != nullptr)
                    {
                        cur = cur->right;
                    }
                    else
                    {   //create a new node
                        Node* n = new Node(key, value);
                        cur ->right = n;
                        return;
                    }
                }
            }
        }
    }

    Iterator find(const KeyType& key) const
    {   //creates a new iterator
        return Iterator(this->head, key);
    }

  private:
    
    struct Node
    {
        Node(KeyType k, ValueType v )
        {
            key = k;
            values.push_back(v);
            right = nullptr;
            left = nullptr;
        }
        KeyType key;
        std::vector<ValueType> values;
        Node* right;
        Node* left;
    };
    
    Node* head;
    
    void treeDies(Node* head);
    Node* getHead();

};

//deleting the tree
template <typename KeyType, typename ValueType>
void TreeMultimap<KeyType,ValueType>::treeDies(Node* head)
{
    if(head == nullptr)
    {
        return;
    }
    
    treeDies(head->left);
    treeDies(head->right);
    delete head;
}


#endif // TREEMULTIMAP_INCLUDED
