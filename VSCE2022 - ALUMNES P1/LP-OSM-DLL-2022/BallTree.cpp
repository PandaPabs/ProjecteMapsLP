#include "pch.h"
#include "BallTree.h"
using std::list;

Coordinate BallTree::puntMaxLlunya(const Coordinate& ref,const std::vector<Coordinate>& punts)
{
    double maxDist = -1.0;
    Coordinate resultat = punts[0];
    Util util;

    for (const auto& a : punts)
    {
        double d = util.DistanciaHaversine(ref, a);
        if (d > maxDist)
        {
            maxDist = d;
            resultat = a;
        }
    }
    return resultat;
}

void BallTree::construirArbre(const std::vector<Coordinate>& coordenades) {
    m_coordenades = coordenades; // guarda coordenades
    Util util;
  
    if (coordenades.size() == 1) // si bola final para
    {
        m_pivot = coordenades[0];
        m_radi = 0.0;
        m_left = nullptr;
        m_right = nullptr;
        return;
    }

    
    m_pivot = coordenades[0]; //pposem el primer punt com a pivot

    
    Coordinate A = puntMaxLlunya(m_pivot, coordenades); // punt més llunya al pivot

    
    Coordinate B = puntMaxLlunya(A, coordenades); // punt més llunya a A

    
    m_radi = util.DistanciaHaversine(m_pivot, A); // fer el radi

    
    std::vector<Coordinate> esquerra;
    std::vector<Coordinate> dreta;

    for (const auto& a : coordenades)//mira a quina bola s'en va, la més propera
    {
        double de = util.DistanciaHaversine(a, A);
        double dd = util.DistanciaHaversine(a, B);

        if (de < dd) 
        {
            esquerra.push_back(a);
        }    
        else 
        {
            dreta.push_back(a);
        }

    }

    

    
    m_left = new BallTree();
    m_left->construirArbre(esquerra);

    m_right = new BallTree();
    m_right->construirArbre(dreta);

    
    if (m_root == nullptr) 
    {
        m_root = this;
    }
        
}

void BallTree::inOrdre(std::vector<std::list<Coordinate>>& out) {

    if (m_left != nullptr) //miro esquerra
    {
        m_left->inOrdre(out);
    }
        
    out.push_back(list<Coordinate>(m_coordenades.begin(),m_coordenades.end())); //afegir a out

    if (m_right != nullptr) //miro dreta
    {
        m_right->inOrdre(out);
    }
}
void BallTree::preOrdre(std::vector<std::list<Coordinate>>& out) {


    out.push_back(list<Coordinate>(m_coordenades.begin(), m_coordenades.end())); //afegir a out

    if (m_left != nullptr) //miro esquerra
    {
        m_left->inOrdre(out);
    }

    if (m_right != nullptr) //miro dreta
    {
        m_right->inOrdre(out);
    }
}

void BallTree::postOrdre(std::vector<std::list<Coordinate>>& out) {  

    

    if (m_left != nullptr) //miro esquerra
    {
        m_left->inOrdre(out);
    }

    if (m_right != nullptr) //miro dreta
    {
        m_right->inOrdre(out);
    }

    out.push_back(list<Coordinate>(m_coordenades.begin(), m_coordenades.end())); //afegir a out
}

Coordinate BallTree::nodeMesProper(Coordinate targetQuery, Coordinate& Q, BallTree* ball) {
    if (ball == nullptr) // caso base la bola esta vacia 
    {
        return Q;
    }
    Util util;

    double d1 = util.DistanciaHaversine(targetQuery, ball->getPivot()); // distancia desde el punto deseado y al pivote de la bola
    double d2 = util.DistanciaHaversine(targetQuery, Q); // distancia desde el punto deseado y el punto más cercano actual

    if (d1 - ball->getRadi() >= d2) // si la distancia mínima a un punto de la bola es más grande que la distancia al punta más cercano actual no buscamos 
    {
        return Q;
    }


    if (ball->getEsquerre() == nullptr && ball->getDreta() == nullptr) // no tiene hijos
    {
        for (const auto& a : ball->getCoordenades())
        {
            if (util.DistanciaHaversine(targetQuery, a) < util.DistanciaHaversine(targetQuery, Q)) // mira si hay un punto de la hoja que es más cercano que el actual
            {
                Q = a;
            }
        }
        return Q;
    }

    double de = util.DistanciaHaversine(targetQuery, ball->getEsquerre()->getPivot()); // distancia al pivot de la izquierda

    double dd = util.DistanciaHaversine(targetQuery, ball->getDreta()->getPivot()); //

    if (de < dd)
    {
        nodeMesProper(targetQuery, Q, ball->getEsquerre());
        nodeMesProper(targetQuery, Q, ball->getDreta());
    }
    else
    {
        nodeMesProper(targetQuery, Q, ball->getDreta());
        nodeMesProper(targetQuery, Q, ball->getEsquerre());
    }

    return Q; 
}



