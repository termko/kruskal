#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

template <typename T>
class Array {
private:
    T* ptr;
    int size;
  
public:
    Array(T arr[], int s);
    Array();
    void push_back(T elem);
    int getSize();
    T& operator[] (const int index);
};
  
template <typename T>
Array<T>::Array(T arr[], int s)
{
    ptr = new T[s];
    size = s;
    for (int i = 0; i < size; i++)
        ptr[i] = arr[i];
}

template <typename T>
Array<T>::Array()
{
    size = 0;
    ptr = NULL;
}

template <typename T>
void Array<T>::push_back(T elem)
{
    T *res;

    size++;
    res = new T[size];
    if (ptr != NULL) {
        for (int i = 0; i < size - 1; i++) res[i] = ptr[i];
    }
    res[size - 1] = elem;
    ptr = res;
}

template <typename T>
int Array<T>::getSize()
{
    return size;
}

template <typename T>
T& Array<T>::operator[] (const int index)
{
    return ptr[index];
}

class Node {
    public:
        Node(std::string name) {
            this->name = name;
            this->neighbours = new Array <Node*>;
            this->neighbours->push_back(this);
        };
        bool hasNeighbour(std::string name);
        void mergeNeighbours(Node *that);

        std::string name;
        Array <Node*> *neighbours;
};

class Link {
    public:
        Link(Node *a, Node *b, int weight) {
            this->a = a;
            this->b = b;
            this->weight = weight;
        };
        Link() {
            this->a = NULL;
            this->b = NULL;
            this->weight = 0;
        };

        Node *a;
        Node *b;
        int weight;
};

bool Node::hasNeighbour(std::string name) {
    for (int i = 0; i < neighbours->getSize(); i++) {
        if ((*neighbours)[i]->name == name)
            return true;
    }
    return false;
}

void Node::mergeNeighbours(Node *that) {
    for (int i = 0; i < that->neighbours->getSize(); i++) {
        if (!hasNeighbour((*that->neighbours)[i]->name)) {
            neighbours->push_back((*that->neighbours)[i]);
        }
    }
    for (int i = 0; i < neighbours->getSize(); i++) {
        Node *node = (*neighbours)[i];
        node->neighbours = neighbours;
    }

    that->neighbours = neighbours;
}

Array <std::string> stringSplit(std::string str, std::string delimeter) {
    Array <std::string> result;

    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimeter)) != std::string::npos) {
        token = str.substr(0, pos);
        result.push_back(token);
        str.erase(0, pos + delimeter.length());
    }
    result.push_back(str);

    return result;
}


void sortLinks(Array <Link> *graph, bool cmp(Link a, Link b)) {
    for (int i = 0; i < graph->getSize(); i++) {
        for (int j = i; j < graph->getSize(); j++) {
            if (i == j)
                continue;
            if (cmp((*graph)[i], (*graph)[j])) {
                Link tmp = (*graph)[i];
                (*graph)[i] = (*graph)[j];
                (*graph)[j] = tmp;
            }
        }
    }
}

bool compareByWeight(Link a, Link b)
{
    return a.weight > b.weight;
}

bool compareByName(Link a, Link b)
{
    return a.a->name > b.a->name || (a.a->name == b.a->name && a.b->name > a.b->name);
}

Array <Link> parseStringVector(Array <std::string> strings) {
    Array <Link> result;
    Array <Node*> nodes;

    for (int i = 0; i < strings.getSize(); i++) {
        Array <std::string> split = stringSplit(strings[i], " ");
        if (split.getSize() != 3) {
            std::cout << "Wrong format of input on string " << i + 1 << ":\n" << strings[i] << "\n";
            exit(1);
        }
        Node *a = NULL, *b = NULL;
        for (int i = 0; i < nodes.getSize(); i++) {
            if (nodes[i]->name == split[0]) {
                a = nodes[i];
            }
            if (nodes[i]->name == split[1]) {
                b = nodes[i];
            }
            if (a != NULL && b != NULL) {
                break;
            }
        }
        if (a == NULL) {
            a = new Node(split[0]);
            nodes.push_back(a);
        }
        if (b == NULL) {
            b = new Node(split[1]);
            nodes.push_back(b);
        }

        result.push_back(Link(a, b, std::atoi(split[2].c_str())));
    }
    sortLinks(&result, compareByWeight);
    return result;
}

Array <Link> readFromStdin(int ac, char *av[]) {
    if (ac == 2) {
        std::ifstream t(av[1]);
        std::stringstream buffer;
        buffer << t.rdbuf();
        t.close();

        return parseStringVector(stringSplit(buffer.str(), "\n"));
    }
    Array <std::string> buffer;
    for (std::string line; std::getline(std::cin, line);) {
        buffer.push_back(line);
    }
    return parseStringVector(buffer);
}

int main(int ac, char *av[]) {
    Array <Link> graph = readFromStdin(ac, av);
    int totalWeight = 0;
    Array <Link> result;

    for (int i = 0; i < graph.getSize(); i++) {
        Node *a = graph[i].a, *b = graph[i].b;
        int weight = graph[i].weight;
        if (a->neighbours == b->neighbours) {
            continue;
        }

        a->mergeNeighbours(b);
        totalWeight += weight;
        result.push_back(graph[i]);
    }

    sortLinks(&result, compareByName);
    for (int i = 0; i < result.getSize(); i++) {
        std::cout << result[i].a->name << " " << result[i].b->name << "\n";
    }
    std::cout << totalWeight << "\n";
}