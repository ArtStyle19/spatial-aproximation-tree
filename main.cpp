#include <GLFW/glfw3.h>
#include <GL/glut.h> // Include GLUT for text rendering
#include <utility>
#include <vector>
#include <cmath>
#include <iostream>
#include <cstdlib> // For std::rand() and std::srand()
#include <ctime>   // For std::time>
#include <algorithm>

// class Node {
// public:
//     float x, y;
//     std::vector<Node*> children;
//     Node(float x, float y) : x(x), y(y) {}
// };

// class SATree {
// public:
//     Node* root;
//     SATree() {
//         root = nullptr;
//     }

//     Node* buildSa(std::vector<std::pair<float, float>>& S) {
//         if (S.empty()) return nullptr;
//         root = new Node(S[10].first, S[10].second);
//         S.erase(S.begin());
//         return buildSaTree(S, root);
//     }

//     Node* buildSaTree(std::vector<std::pair<float, float>>& S, Node* curr_node) {
//         if (S.empty()) return curr_node;

//         while (!S.empty()) {
//             float min_dist = std::numeric_limits<float>::max();
//             auto closest = S.end();
//             for (auto jt = S.begin(); jt != S.end(); ++jt) {
//                 float dist = distance(curr_node->x, curr_node->y, jt->first, jt->second);
//                 if (dist < min_dist) {
//                     min_dist = dist;
//                     closest = jt;
//                 }
//             }
//             if (closest != S.end()) {
//                 Node* child = new Node(closest->first, closest->second);
//                 curr_node->children.push_back(child);
//                 S.erase(closest); // Remove the closest point from the set
//                 std::cout << closest->first << closest->second << '\n';
//                 buildSaTree(S, child);
//             }
//         }
//         return curr_node;
//     }

// private:
//     float distance(float x1, float y1, float x2, float y2) {
//         return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
//     }
// };
//
//
//
class Node {
public:
    float x, y;
    float R; // Covering radius
    std::vector<Node*> neighbors;
    std::vector<Node*> children;

    Node(float x, float y) : x(x), y(y), R(0) {}
};

class SATree {
public:
    Node* root;
    SATree() {
        root = nullptr;
    }

    Node* buildSaTree(std::vector<std::pair<float, float>>& S) {
        if (S.empty()) return nullptr;
        Node* root = new Node(S[0].first, S[0].second);
        S.erase(S.begin());
        buildTree(root, S);
        return root;
    }

// private:
    float distance(float x1, float y1, float x2, float y2) {
        return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    }

    void buildTree(Node* a, std::vector<std::pair<float, float>>& S) {
        std::vector<Node*> N_a; // Neighbors of a
        a->R = 0;

        // Sort S by increasing distance to a
        std::sort(S.begin(), S.end(), [a, this](const std::pair<float, float>& p1, const std::pair<float, float>& p2) {
            return distance(a->x, a->y, p1.first, p1.second) < distance(a->x, a->y, p2.first, p2.second);
        });

        for (const auto& v : S) {
            float d_va = distance(a->x, a->y, v.first, v.second);
            a->R = std::max(a->R, d_va);
            bool addNeighbor = true;
            for (const auto& b : N_a) {
                if (distance(b->x, b->y, v.first, v.second) < d_va) {
                    // addNeighbor = false;
                    addNeighbor = false;
                    break;
                }
            }
            if (addNeighbor) {
                Node* newNode = new Node(v.first, v.second);
                N_a.push_back(newNode);
            }
        }

        // for (auto& b : N_a) {
        //     b->children.clear();
        // }

        std::vector<std::pair<float, float>> S_prime;

        for (const auto& v : S) {
            bool inNeighbors = false;
            for (const auto& b : N_a) {


                float dist_to_a = distance(a->x, a->y, v.first, v.second);
                float dist_to_b = distance(b->x, b->y, v.first, v.second);

                std::cout << "Distance to a: " << dist_to_a << ", Distance to b: " << dist_to_b << std::endl;

                if (dist_to_b < dist_to_a && dist_to_b == 0) {
                    std::cout << "verdad" << '\n' << '\n';
                    // std::cout << distance(b->x, b->y, v.first, v.second) << " xd " << distance(a->x, a->y, v.first, v.second) << '\n';
                    inNeighbors = true;
                    break;
                }
                // if (distance(b->x, b->y, v.first, v.second) < distance(a->x, a->y, v.first, v.second)) {
                //     std::cout << "aea" << '\n';
                //     std::cout << distance(b->x, b->y, v.first, v.second) << " xd " << distance(a->x, a->y, v.first, v.second) << '\n';
                //     inNeighbors = true;
                //     break;
                // }
            }



            if (!inNeighbors) {
                std::cout << std::endl;
                std::cout << "Haciendo algo xd"  << '\n';
                Node* c = *std::min_element(N_a.begin(), N_a.end(), [a, v, this](Node* n1, Node* n2) {
                    return distance(a->x, a->y, n1->x, n1->y) < distance(a->x, a->y, n2->x, n2->y);
                });
                c->children.push_back(new Node(v.first, v.second));
                S_prime.push_back(v);
                break;
            }
        }

        for (const auto& b : N_a) {

            a->children.push_back(b);

            std::cout << b->x << "-------------" <<  b->y << '\n';
            //S_prime empty
            //S all points
            for (auto& childd : S_prime) {
                std::cout << childd.first << childd.second << '\n';
            }
            buildTree(b, S_prime);
        }
    }
};

void drawNode(Node* node) {
    if (!node) return;

// Draw the node as a circle
    // std:: cout << node->x << node->y << '\n';
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.14159f / 180;
        glVertex2f(node->x + 5 * cos(theta), node->y + 5 * sin(theta));
    }

    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f); // Set color to red (R=1, G=0, B=0)
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.14159f / 180;
        glVertex2f(node->x + node->R * cos(theta), node->y + node->R * sin(theta));
    }
    glEnd();


    glColor3f(0.0f, 1.0f, 0.0f); // Set color to red (R=1, G=0, B=0)
    glBegin(GL_LINES);
    for (auto child : node->children) {
        glVertex2f(node->x, node->y);
        glVertex2f(child->x, child->y);
    }
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f); // Set color to red (R=1, G=0, B=0)


    for (auto child : node->children) {
        // std::cout <<  child->x << '-' << child->y << "draw" << '\n';
        drawNode(child);
    }

}

void render(Node* root, std::vector<std::pair<float, float>>& S) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawNode(root);

    for (auto node : S) {
    glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i++) {
            float theta = i * 3.14159f / 180;
            glVertex2f(node.first + 5 * cos(theta), node.second + 5 * sin(theta));
        }
    glEnd();
    }


    glfwSwapBuffers(glfwGetCurrentContext());
}

int main(int argc, char** argv) {


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Spatial Approximation Tree", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    gluOrtho2D(0, 1000, 1000, 0);

    std::vector<std::pair<float, float>> S = {
        // {100, 100}, {200, 150}, {300, 200}, {400, 250}, {500, 300},
        // {600, 350}, {700, 400}, {800, 450}, {900, 500}, {100, 550},
        // {200, 600}, {300, 650}, {400, 700}, {500, 750}, {600, 800},
        // {700, 850}, {800, 900}, {900, 950}, {100, 200}, {200, 400},
        // {300, 600}, {400, 800}, {500, 100}
        {300, 300},
        {176, 200},
        {250, 350},
        {500, 500},
        {100, 500},
        {600, 100},
        {400, 400},
        {400, 200},
        {400, 100},
    };

    SATree tree;
    Node* root = tree.buildSaTree(S);

    while (!glfwWindowShouldClose(window)) {
        render(root ,S );
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

