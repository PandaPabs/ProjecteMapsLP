#include "pch.h"
#include "BallTree.h"
using std::list;

Coordinate BallTree::puntMaxLlunya(const Coordinate& ref, const std::vector<Coordinate>& punts)
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
    //treure repetides
    std::vector<Coordinate> uniques;
    for (const auto& co : coordenades) {
        bool trob = false;
        for (const auto& uniq : uniques) {
            if (co.lat == uniq.lat && co.lon == uniq.lon) {
                trob = true;
                break;
            }
        }
        if (!trob) {
            uniques.push_back(co);
        }
    }
    m_coordenades = uniques;
   
    Util util;

    if (uniques.empty()) {
        m_pivot = Coordinate{ 0.0, 0.0 };
        m_radi = 0.0;
        m_left = nullptr;
        m_right = nullptr;
        return;
    }

    if (uniques.size() == 1) // si bola final para
    {
        m_pivot = uniques[0];
        m_radi = 0.0;
        m_left = nullptr;
        m_right = nullptr;
        return;
    }


    //posem el punt mig de pivot
    double sumaLat = 0.0, sumaLon = 0.0;
    for (const auto& co : uniques) {
        sumaLat += co.lat;
        sumaLon += co.lon;
    }
    m_pivot = { sumaLat / uniques.size(), sumaLon / uniques.size() };


    Coordinate A = puntMaxLlunya(m_pivot, uniques); // punt més llunya al pivot


    Coordinate B = puntMaxLlunya(A, uniques); // punt més llunya a A


    m_radi = util.DistanciaHaversine(m_pivot, A); // fer el radi


    std::vector<Coordinate> esquerra;
    std::vector<Coordinate> dreta;

    for (const auto& a : uniques)//mira a quina bola s'en va, la més propera
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

    if ((esquerra.empty() || dreta.empty()) || (esquerra.size() == uniques.size() || dreta.size() == uniques.size())){
        m_left = nullptr;
        m_right = nullptr;
        double maxDist = 0.0;
        for (const auto& coord : uniques) {
            double dist = util.DistanciaHaversine(m_pivot, coord);
            if (dist > maxDist) maxDist = dist;
        }
        m_radi = maxDist;
        return;
    }


    m_left = new BallTree();
    m_left->construirArbre(esquerra);

    m_right = new BallTree();
    m_right->construirArbre(dreta);


    if (m_root == nullptr)
    {
        m_root = this;
    }
    if (m_left != nullptr) {
        m_left->setArrel(m_root);
    }
    if (m_right != nullptr) m_right->setArrel(m_root);

}

void BallTree::inOrdre(std::vector<std::list<Coordinate>>& out) {

    if (m_left != nullptr) //miro esquerra
    {
        m_left->inOrdre(out);
    }

    out.push_back(list<Coordinate>(m_coordenades.begin(), m_coordenades.end())); //afegir a out

    if (m_right != nullptr) //miro dreta
    {
        m_right->inOrdre(out);
    }
}
void BallTree::preOrdre(std::vector<std::list<Coordinate>>& out) {


    out.push_back(list<Coordinate>(m_coordenades.begin(), m_coordenades.end())); //afegir a out

    if (m_left != nullptr) //miro esquerra
    {
        m_left->preOrdre(out);
    }

    if (m_right != nullptr) //miro dreta
    {
        m_right->preOrdre(out);
    }
}

void BallTree::postOrdre(std::vector<std::list<Coordinate>>& out) {



    if (m_left != nullptr) //miro esquerra
    {
        m_left->postOrdre(out);
    }

    if (m_right != nullptr) //miro dreta
    {
        m_right->postOrdre(out);
    }

    out.push_back(list<Coordinate>(m_coordenades.begin(), m_coordenades.end())); //afegir a out
}

Coordinate BallTree::nodeMesProper(Coordinate targetQuery, Coordinate& Q, BallTree* ball) {
    if (ball == nullptr) //no bola
        return Q;
    
    
    Util util;

    double d1 = util.DistanciaHaversine(ball->getPivot(), targetQuery); 
    // Calcula la distància del punt central de la bola respecte al pdi (D1)
    double d2 = util.DistanciaHaversine(targetQuery, Q); 
    // Calcula la distància del pdi respecte al Q (D2)

    if (d1 - ball->getRadi() >= d2) // Si D1 – bola.radi >= D2  retorna Q
    {
        return Q;
    }


    if (ball->getEsquerre() == nullptr && ball->getDreta() == nullptr) // IF fulla
    {
        // actualitza Q si és el node camí més proper al punt d’interès, dels punts que formen la bola
        for (const auto& a : ball->getCoordenades())
        {
            double dist = util.DistanciaHaversine(targetQuery, a);
            if (dist < d2) {
                Q = a;
                d2 = dist;
            }
        }

    }
    else {
        double Da = util.DistanciaHaversine(targetQuery, ball->getEsquerre()->getPivot()); //Calcula la distància pdi respecte el punt central de la bola esquerre (Da)


        double Db = util.DistanciaHaversine(targetQuery, ball->getDreta()->getPivot()); //Calcula la distància pdi respecte del punt central de la bola dreta (Db
       
        if (Da < Db) {
            if (ball->getEsquerre() != nullptr) {
                Q = nodeMesProper(targetQuery, Q, ball->getEsquerre());
                d2 = util.DistanciaHaversine(targetQuery, Q);
            }
            if (ball->getDreta() != nullptr && Db - ball->getDreta()->getRadi() < d2) {
                Q = nodeMesProper(targetQuery, Q, ball->getDreta());
            }
        }
        else {
            if (ball->getDreta() != nullptr) {
                Q = nodeMesProper(targetQuery, Q, ball->getDreta());
                d2 = util.DistanciaHaversine(targetQuery, Q);
            }
            if (ball->getEsquerre() != nullptr && Da - ball->getEsquerre()->getRadi() < d2) {
                Q = nodeMesProper(targetQuery, Q, ball->getEsquerre());
            }
        }
    }
    return Q;
}