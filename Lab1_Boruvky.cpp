// Include header file
#include<iostream>
#include<vector>
#include<fstream>
using namespace std;

class Edge
{
public:
    int weight;
    int dest;
    int src;
    Edge* next;

    Edge(int weight, int src, int dest)
    {
        this->weight = weight;
        this->dest = dest;
        this->src = src;
        this->next = NULL;
    }
};
class State
{
public: int parent;
      int rank;
      State(int parent, int rank)
      {
          this->parent = parent;
          this->rank = rank;
      }
};
class Graph
{
public: int vertices;
      vector < vector < Edge*> > graphEdge;
      Graph(int vertices)
      {
          this->vertices = vertices;

          for (int i = 0; i < this->vertices; ++i)
          {
              this->graphEdge.push_back(vector < Edge*>());
          }
      }
      void addEdge(int src, int dest, int w)
      {
          if (dest < 0 || dest >= this->vertices ||
              src < 0 || src >= this->vertices)
          {
              return;
          }

          this->graphEdge.at(src).push_back(new Edge(w, src, dest));
          if (dest == src)
          {
              return;
          }
          this->graphEdge.at(dest).push_back(new Edge(w, dest, src));
      }

      int find(State** subsets, int i)
      {
          if (subsets[i]->parent != i)
          {
              subsets[i]->parent = this->find(subsets, subsets[i]->parent);
          }
          return subsets[i]->parent;
      }
      void findUnion(State** subsets, int x, int y)
      {
          int a = this->find(subsets, x);
          int b = this->find(subsets, y);
          if (subsets[a]->rank < subsets[b]->rank)
          {
              subsets[a]->parent = b;
          }
          else if (subsets[a]->rank > subsets[b]->rank)
          {
              subsets[b]->parent = a;
          }
          else
          {
              subsets[b]->parent = a;
              subsets[a]->rank++;
          }
      }
      void boruvkaMST()
      {
          cout << "Edges in Minimum Spanning Tree" << endl;
          int result = 0;
          int selector = this->vertices;
          State** subsets = new State * [this->vertices];
          Edge** cheapest = new Edge * [this->vertices];
          for (int v = 0; v < this->vertices; ++v)
          {
              subsets[v] = new State(v, 0);
          }
          while (selector > 1)
          {
              for (int v = 0; v < this->vertices; ++v)
              {
                  cheapest[v] = NULL;
              }
              for (int k = 0; k < this->vertices; k++)
              {
                  for (int i = 0; i < this->graphEdge.at(k).size(); ++i)
                  {
                      int set1 = this->find(subsets,
                          this->graphEdge.at(k).at(i)->src);
                      int set2 = this->find(subsets,
                          this->graphEdge.at(k).at(i)->dest);
                      if (set1 != set2)
                      {
                          if (cheapest[k] == NULL)
                          {
                              cheapest[k] = this->graphEdge.at(k).at(i);
                          }
                          else if (cheapest[k]->weight >
                              this->graphEdge.at(k).at(i)->weight)
                          {
                              cheapest[k] = this->graphEdge.at(k).at(i);
                          }
                      }
                  }
              }
              for (int i = 0; i < this->vertices; i++)
              {
                  if (cheapest[i] != NULL)
                  {
                      int set1 = this->find(subsets, cheapest[i]->src);
                      int set2 = this->find(subsets, cheapest[i]->dest);
                      if (set1 != set2)
                      {
                          selector--;
                          this->findUnion(subsets, set1, set2);
                          cout << "(" << cheapest[i]->src << " - " << cheapest[i]->dest << "), weight = " << cheapest[i]->weight << endl;
                          result += cheapest[i]->weight;
                      }
                  }
              }
          }
          cout << "Weight of Minimum Spanning Tree is " << result << endl;
      }
};
int main()
{
    ifstream in("l1_3.txt");
    int n;
    in >> n;
    Graph* G = new Graph(n);

    int matr[8][8];

    if (in.is_open())
    {
        int a, b, c;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                in >> matr[i][j];
            }
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if(matr[i][j] > 0)
                    G->addEdge(i, j, matr[i][j]);
            }
        }
    }

    G->boruvkaMST();

    in.close();
    return 0;
}