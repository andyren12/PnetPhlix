#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <vector>

template <typename KeyType, typename ValueType>
class TreeMultimap
{
    struct Node;
public:
    class Iterator
    {
      public:
        Iterator() : it(nullptr), pos(0)
        {
            values.clear();
        }
    
        Iterator(Node* curr) : pos(0)
        {
            values = curr->mValues;
            it = &curr->mValues[0];
        }

        ValueType& get_value() const
        {
            return *it;
        }

        bool is_valid() const
        {
            return pos < values.size();
        }

        void advance()
        {
            pos++;
            it = &values[pos];
            if (!is_valid()) return;
        }

      private:
        ValueType* it;
        std::vector<ValueType> values;
        int pos;
    };

    TreeMultimap()
    {
        root = nullptr;
    }

    ~TreeMultimap()
    {
        clear(root);
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        if (root == nullptr)
        {
            root = new Node(key, value);
            return;
        }
        
        Node* curr = root;
        for (;;)
        {
            if (key == curr->mKey)
            {
                curr->mValues.push_back(value);
                return;
            }
            if (key < curr->mKey)
            {
                if (curr->less != nullptr) curr = curr->less;
                else
                {
                    curr->less = new Node(key, value);
                    return;
                }
            } else if (key > curr->mKey)
            {
                if (curr->greater != nullptr) curr = curr->greater;
                else
                {
                    curr->greater = new Node(key, value);
                    return;
                }
            }
        }
    }

    Iterator find(const KeyType& key) const
    {
        Node* curr = getNode(root, key);
        return Iterator(curr);
    }
    
private:
    struct Node {
        Node(KeyType key, ValueType value) : mKey(key), less(nullptr), greater(nullptr)
        {
            mValues.push_back(value);
        }
        KeyType mKey;
        std::vector<ValueType> mValues;
        Node* less;
        Node* greater;
    };
    
    Node* root;
    
    Node* getNode(Node* node, const KeyType& key) const
    {
        if (node == nullptr) return nullptr;
        if (node->mKey == key) return node;
        else if (node->mKey > key) return getNode(node->less, key);
        else return getNode(node->greater, key);
    };
    
    void clear(Node* node)
    {
        if (node == nullptr) return;
        clear(node->less);
        clear(node->greater);
        delete node;
    }
    
};

#endif // TREEMULTIMAP_INCLUDED
