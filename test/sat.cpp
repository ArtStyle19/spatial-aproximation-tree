#include <GLFW/glfw3.h>
#include <GL/glut.h> // Include GLUT for text rendering
#include <utility>
#include <vector>
#include <cmath>
#include <iostream>
#include <cstdlib> // For std::rand() and std::srand()
#include <ctime>   // For std::time>
#include <algorithm>
#include <set>


std::vector<std::pair<float, float>> S = {
    // {50, 100},
    // {176, 136},
    //
    // {300, 200},
    // {400, 250},
    // {500, 300},
    // {600, 350},
    // {700, 400},
    //
    // {769, 478},
    // {923, 514},
    // {97, 532},
    // {193, 578},
    // {322, 672},
    // {423, 723},
    // {500, 750},
    //
    
    // {600, 800},
    // {700, 850},

    // {800, 900},
    // {900, 950},
    // {100, 200}, 
    // {200, 400},
    // {300, 600},
    // {400, 800},
    // {500, 100}



    // {300, 300},
    // {176, 200},
    // {250, 350},
    // {500, 500},
    // {100, 500},
    // {600, 100},
    // {400, 400},
    // {400, 200},
    // {400, 100},
    // {0, 0},
    // {100, 800},
    // {200, 300},
    // {300, 200},
    // {700, 190},
    // {900, 100}
    //
    //

    // Ejemplo 1

    {100,300},
    {200,500},
    {300,100},
    {500,400},

    {600,200},

    {700,700},
    {800,600},
    {900,300},
    {400,800},
    {500,600}


    // Ejemplo 2
//     {100, 200},
// {200, 400},
// {300, 600},
// {400, 800},
// {500, 100},
// {600, 300},
// {700, 500},
// {800, 700},
// {900, 900},
// {100, 400}

    //Ejemplo 3
//     {700, 100},
// {600, 400},
// {100, 600},
// {200, 900},
// {400, 300},
// {800, 200},
// {300, 800},
// {900, 500},
// {500, 700},
// {300, 600}


    //Ejemplo 4
//     {400, 800},
// {700, 300},
// {200, 600},
// {100, 900},
// {900, 100},
// {500, 200},
// {600, 700},
// {300, 500},
// {800, 400},
// {200, 800}






    // {360, 410}, //p11
    // {430,710}, //p1
    // {700, 290}, //p2
    // {240, 100},// p3
    // {365, 300},// p4
    // {210, 550},// p5
    // {450, 420},// p6
    // {260, 440},// p7
    // {740, 810}, //p8
    // {700, 500}, //p9
    // {580, 370}, //p10
    // {530, 170}, //p12
    // {510, 780}, //p13
    // {430, 500}, //p14
    // {390, 580}, //p15

    // {100, 700},
};

class Node {
public:
    float x, y;
    float R; // Covering radius
    std::vector<Node*> neighbors;
    std::vector<Node*> children;

    Node(float x, float y) : x(x), y(y), R(0) {}
};

std::vector<std::pair<float, float>> S_prime;
class SATree {
public:
    Node* root;

    SATree() : root(nullptr) {}

    Node* buildSaTree(std::vector<std::pair<float, float>>& S) {
        if (S.empty()) return nullptr;
        srand(time(NULL));
        // int rando = rand() % S.size();
        int rando = 3;
        // int rando = 0;

        std::cout << rando << '\n';
        root = new Node(S[rando].first, S[rando].second);
        S.erase(S.begin() + rando);
        buildTree(root, S);
        return root;
    }

    std::vector<std::pair<float, float>> rangeSearch(Node* node, float qx, float qy, float r) {
        std::set<std::pair<float, float>> uniqueResults;
        rangeSearchHelper(node, qx, qy, r, uniqueResults);
        return std::vector<std::pair<float, float>>(uniqueResults.begin(), uniqueResults.end());
    }

private:
    float distance(float x1, float y1, float x2, float y2) {
        return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    }

    void rangeSearchHelper(Node* node, float qx, float qy, float r, std::set<std::pair<float, float>>& result) {
        if (!node) return;

        // Distancia del nodo a la consulta
        float d = distance(node->x, node->y, qx, qy);

        // Si el nodo está dentro del rango, añadirlo al resultado
        if (d <= r) {
            result.insert({node->x, node->y});
        }

        // Recorrer los hijos y vecinos si están dentro del rango de búsqueda extendido
        for (Node* child : node->children) {
            if (distance(child->x, child->y, qx, qy) <= r + child->R) {
                rangeSearchHelper(child, qx, qy, r, result);
            }
        }
    }
    void buildTree(Node* a, std::vector<std::pair<float, float>>& S) {
        std::vector<Node*> N_a; // Neighbors of a
        a->R = 0;

        // Sort S by increasing distance to a
        std::sort(S.begin(), S.end(), [a, this](const std::pair<float, float>& p1, const std::pair<float, float>& p2) {
            return distance(a->x, a->y, p1.first, p1.second) < distance(a->x, a->y, p2.first, p2.second);
        });
        std::cout << "Caso -> "<< a->x << ','<< a->y << '\n';
        std::cout << "Elementos Disponibles " << ' ';
        for (const auto& v : S) {
            std::cout << '{'<<v.first << ',' << v.second << '}' << ' ';
        }
        std::cout << '\n';

        // Determine neighbors N(a)
        for (const auto& v : S) {
            float d_va = distance(a->x, a->y, v.first, v.second);
            a->R = std::max(a->R, d_va);
            bool addNeighbor = true;
            for (const auto& b : N_a) {
                if (distance(b->x, b->y, v.first, v.second) < d_va) {
                    addNeighbor = false;
                    break;
                }
            }
            if (addNeighbor) {
                Node* newNode = new Node(v.first, v.second);
                N_a.push_back(newNode);
            }
        }

        // Clear children of neighbors
        for (auto& b : N_a) {
            b->children.clear();
        }


        // Assign points to appropriate neighbors and recursively build tree
        for (const auto& v : S) {
            bool inNeighbors = false;
            for (const auto& b : N_a) {
                float dist_to_a = distance(a->x, a->y, v.first, v.second);
                float dist_to_b = distance(b->x, b->y, v.first, v.second);
                if (dist_to_b < dist_to_a) {
                    inNeighbors = true;
                    break;
                }
            }
            if (!inNeighbors) {
                // Find the closest node in N_a to insert v into its children
                Node* closestNode = *std::min_element(N_a.begin(), N_a.end(), [a, v, this](Node* n1, Node* n2) {
                    return distance(a->x, a->y, n1->x, n1->y) < distance(a->x, a->y, n2->x, n2->y);
                });
                // std::cout << closestNode->x << closestNode ->y << '\n';
                closestNode->children.push_back(new Node(v.first, v.second));

                std::cout << "Elegido" << v.first << ',' << v.second << '\n';
                S_prime.push_back(v);
            }
        }

        // Recursively build tree for each neighbor in N_a
        for (const auto& b : N_a) {
            std::vector<std::pair<float, float>> S_b;
            for (const auto& v : S) {
                if (std::find(S_prime.begin(), S_prime.end(), v) == S_prime.end()) {
                    float dist_to_b = distance(b->x, b->y, v.first, v.second);
                    if (dist_to_b < distance(a->x, a->y, v.first, v.second)) {
                        S_b.push_back(v);
                    } else {
                        std::cout << '\n';
                        std:: cout << "Perdida {" << v.first << ',' << v.second << "} "<< '\n';
                        std::cout << '\n';
                    }
                } else {
                    // std:: cout << "Perdida {" << v.first << ',' << v.second << '\n';
                }
            }
            a->children.push_back(b);

            buildTree(b, S_b);
        }    }
};

void renderText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void drawNode(Node* node, int depth) {
    if (!node) return;  // Safety check
    //
    //

    GLfloat colors[][3] = {
        {1.0f, 0.0f, 0.0f},  // Red for depth 0
        {0.0f, 1.0f, 0.0f},  // Green for depth 1
        {0.0f, 0.0f, 1.0f},  // Blue for depth 2
        {1.0f, 1.0f, 0.0f},  // Yellow for depth 3
        {1.0f, 0.0f, 1.0f}   // Magenta for depth 4
        // Add more colors as needed
    };
    int colorIndex = depth % (sizeof(colors) / sizeof(colors[0]));



    // Draw the node as a circle
    // glColor3f(1.0f, 1.0f, 1.0f);  // Set color to white
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.14159f / 180;
        glVertex2f(node->x + 10 * cos(theta), node->y + 10 * sin(theta));  // Adjust radius as needed
    }
    glEnd();

    // Draw covering radius as a circle
    // glColor3f(0.0f, 0.0f, 1.0f);  // Set color to blue
    // glBegin(GL_LINE_LOOP);
    // for (int i = 0; i < 360; i++) {
    //     float theta = i * 3.14159f / 180;
    //     glVertex2f(node->x + node->R * cos(theta), node->y + node->R * sin(theta));
    // }
    // glEnd();

    // Draw edges to children
    // glColor3f(0.0f, 1.0f, 0.0f);  // Set color to green
    // Determine color index based on depth (modulo to cycle colors if more depths than colors)
    glColor3f(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);
    glBegin(GL_LINES);
    for (auto child : node->children) {
        glVertex2f(node->x, node->y);
        glVertex2f(child->x, child->y);
    }
    glEnd();

    // Recursively draw children nodes
    glColor3f(1.0f, 1.0f, 1.0f);  // Reset color to white
    for (auto child : node->children) {
        drawNode(child, depth + 1);
    }
}
void render(Node* root, std::vector<std::pair<float, float>>& S) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


     glColor3f(0.7f, 0.7f, 0.7f);  // Set color to gray

    glBegin(GL_LINES);
    // Draw vertical lines
    for (float x = 0; x <= 1000; x += 100) {
        glVertex2f(x, 0);
        glVertex2f(x, 1000);
    }
        // glColor3f(1.0f, 1.0f, 1.0f);  // Set color to white
    // Draw horizontal lines
    for (float y = 0; y <= 1000; y += 100) {
        glVertex2f(0, y);
        glVertex2f( 1000, y);
    }
    glEnd();

    drawNode(root, 0);

    for (auto node : S) {
    glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i++) {
            float theta = i * 3.14159f / 180;
            glVertex2f(node.first + 10 * cos(theta), node.second + 10 * sin(theta));
        }
    glEnd();
    }


    glfwSwapBuffers(glfwGetCurrentContext());
}

void render2(Node* root, std::vector<std::pair<float, float>>& S) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // drawNode(root, 0);
    //
    //
    //

        if (!root) return;


        for (Node* child :root->children) {
    
        // renderText(x);

        }



    glfwSwapBuffers(glfwGetCurrentContext());
}


void draw_query(float x, float y, int r) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // Draw the node as a circle
    // glColor3f(1.0f, 1.0f, 1.0f);  // Set color to white
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.14159f / 180;
        glVertex2f(x + 5 * cos(theta), y + 5 * sin(theta));  // Adjust radius as needed
    }
    glEnd();


    // Establece grosor de línea más grueso
    glLineWidth(3.0f); // Línea más gruesa (ajusta el valor según necesites)
    
    // Dibuja círculo de radio
    glColor3f(1.0f, 1.0f, 1.0f); // Verde brillante (cambiado de magenta)
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.14159f / 180;
        glVertex2f(x + r * cos(theta), y + r * sin(theta));
    }

    glEnd();

    glLineWidth(1.0f);





    glfwSwapBuffers(glfwGetCurrentContext());
}







SATree tree;
Node* root = tree.buildSaTree(S);


void clearTree(Node* node) {
    if (!node) return;

    // Recursivamente eliminar hijos
    for (auto child : node->children) {
        clearTree(child);
    }

    // Limpiar la memoria del nodo actual
    delete node;
}
float x, y, r;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Convertir coordenadas de pantalla a coordenadas de OpenGL
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        x = (float)xpos / width * 1000;  // 1000 es el ancho de la ventana OpenGL
        y = (float)ypos / height * 1000;  // Invertir y para que coincida con el sistema de coordenadas OpenGL


        r = 200;
        std::vector<std::pair<float, float>> results = tree.rangeSearch(root, x, y, r);

        // Imprimir los resultados
        std::cout << "Puntos dentro del rango (" << x << ", " << y << ") con radio " << r << ":\n";
        for (const auto& point : results) {
            std::cout << "(" << point.first << ", " << point.second << ")\n";
        }




        // // Crear un nuevo punto y agregarlo a la lista de puntos S
        // std::pair<float, float> newPoint(x, y);
        // S.push_back(newPoint);

        // // Limpiar el árbol anterior para liberar memoria
        // clearTree(root);
        // root = nullptr;

        // // Reconstruir el árbol con todos los puntos actualizados en S
        // root = tree.buildSaTree(S);

        // // Volver a renderizar con el nuevo árbol
        draw_query(x, y, r);
    }
}

void printTree(Node* node, int depth = 0) {
    if (!node) return;

    // Imprimir la jerarquía del nodo con una sangría según la profundidad
    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }
    std::cout << "(" << node->x << ", " << node->y << ") R=" << node->R << "\n";

    // Recorrer los hijos recursivamente
    for (auto child : node->children) {
        printTree(child, depth + 1);
    }
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

    glfwSetMouseButtonCallback(window, mouse_button_callback);


    while (!glfwWindowShouldClose(window)) {
        // printTree(root);
        render(root ,S );
        draw_query(x, y, r);
        // render2(root, S);

        glfwPollEvents();
    }


    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

