#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

class Node {
    public:
        Node(std::string name) {
            this->name = name;
            this->neighbours = new std::vector <Node*>;
            this->neighbours->push_back(this);
        };
        // Node() {
        //     this->neighbours = new std::vector <Node*>;
        // };
        bool hasNeighbour(std::string name);
        void mergeNeighbours(Node *that);

        std::string name;
        std::vector <Node*> *neighbours;
};

class Link {
    public:
        Link(Node *a, Node *b, int weight) {
            this->a = a;
            this->b = b;
            this->weight = weight;
        };

        Node *a;
        Node *b;
        int weight;
};

bool Node::hasNeighbour(std::string name) {
    for (int i = 0; i < neighbours->size(); i++) {
        if ((*neighbours)[i]->name == name)
            return true;
    }
    return false;
}

void Node::mergeNeighbours(Node *that) {
    for (int i = 0; i < that->neighbours->size(); i++) {
        if (!hasNeighbour((*that->neighbours)[i]->name)) {
            neighbours->push_back((*that->neighbours)[i]);
        }
    }
    for (int i = 0; i < neighbours->size(); i++) {
        Node *node = (*neighbours)[i];
        node->neighbours = neighbours;
    }

    that->neighbours = neighbours;
}

std::vector <std::string> stringSplit(std::string str, std::string delimeter) {
    std::vector <std::string> result;

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

bool compareByWeight(const Link &a, const Link &b)
{
    return a.weight < b.weight;
}

bool compareByName(const Link &a, const Link &b)
{
    return a.a->name < b.a->name || (a.a->name == b.a->name && a.b->name < a.b->name);
}

std::vector <Link> parseStringVector(std::vector <std::string> strings) {
    std::vector <Link> result;
    std::vector <Node*> nodes;

    for (int i = 0; i < strings.size(); i++) {
        std::vector <std::string> split = stringSplit(strings[i], " ");
        if (split.size() != 3) {
            std::cout << "Wrong format of input on string " << i + 1 << ":\n" << strings[i] << "\n";
            exit(1);
        }
        Node *a = NULL, *b = NULL;
        for (int i = 0; i < nodes.size(); i++) {
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
    std::sort(result.begin(), result.end(), compareByWeight);
    return result;
}

std::vector <Link> readFromStdin(int ac, char *av[]) {
    if (ac == 2) {
        std::ifstream t(av[1]);
        std::stringstream buffer;
        buffer << t.rdbuf();
        t.close();

        return parseStringVector(stringSplit(buffer.str(), "\n"));
    }
    std::vector <std::string> buffer;
    for (std::string line; std::getline(std::cin, line);) {
        buffer.push_back(line);
    }
    return parseStringVector(buffer);
}

int main(int ac, char *av[]) {
    std::vector <Link> graph = readFromStdin(ac, av);
    int totalWeight = 0;
    std::vector <Link> result;

    for (int i = 0; i < graph.size(); i++) {
        Node *a = graph[i].a, *b = graph[i].b;
        int weight = graph[i].weight;
        if (a->neighbours == b->neighbours) {
            continue;
        }

        a->mergeNeighbours(b);
        totalWeight += weight;
        result.push_back(graph[i]);
    }

    std::sort(result.begin(), result.end(), compareByName);
    for (int i = 0; i < result.size(); i++) {
        std::cout << result[i].a->name << " " << result[i].b->name << "\n";
    }
    std::cout << totalWeight << "\n";
}