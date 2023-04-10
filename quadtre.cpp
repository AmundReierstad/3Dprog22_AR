 // ADS101 Algoritmer og Datastrukturer for spill 2020
// Dag Nylund, Høgskolen i Innlandet
// Gjort om fra PlainRectangle
#include <iostream>
#include "quadtre.h"

//std::vector<QuadTre> QuadTre::m_rectangles;

QuadTre::QuadTre(const Vector2d &v1, const Vector2d &v2, const Vector2d &v3, const Vector2d &v4)
    : m_a{v1}, m_b{v2}, m_c{v3}, m_d{v4}, m_sv{nullptr}, m_so{nullptr}, m_no{nullptr}, m_nv{nullptr}
{
   //
}

void QuadTre::subDivide(int n) // uniform rekursiv subdivisjon, fyller alle nivåer
{
    if (n>0) {
        Vector2d v1 = (m_a+m_b)/2;
        Vector2d v2 = (m_b+m_c)/2;
        Vector2d v3 = (m_c+m_d)/2;
        Vector2d v4 = (m_d+m_a)/2;
        Vector2d m = (m_a+m_c)/2;
        m_sv = new QuadTre(m_a, v1, m, v4);
        m_sv->subDivide(n-1);
        m_so = new QuadTre(v1, m_b, v2, m);
        m_so->subDivide(n-1);
        m_no = new QuadTre(m, v2, m_c, v3);
        m_no->subDivide(n-1);
        m_nv = new QuadTre(v4, m, v3, m_d);
        m_nv->subDivide(n-1);
    }
}

void QuadTre::print() const
{
    Vector2d c = (m_a+m_c)/2;
    std::cout << "senter = (" << c.x << ", " << c.y << ")" << std::endl;
    for (auto it=m_gameObjects.begin(); it!=m_gameObjects.end(); it++)
        std::cout << it->m_navn << std::endl;
}

bool QuadTre::isLeaf() const
{
   return m_sv == nullptr && m_so == nullptr && m_no == nullptr && m_nv == nullptr;
}

QuadTre *QuadTre::insert(const GameObject &gameObject)
{
    if (isLeaf()) {
        m_gameObjects.push_back(gameObject);
        return this;
    }
    else {
        Vector2d m = (m_a + m_c)/2;
        if (gameObject.getPosition().y < m.y)
        {
            if (gameObject.getPosition().x < m.x)
                m_sv->insert(gameObject);
            else
                m_so->insert(gameObject);
        }
        else {
            if (gameObject.getPosition().x < m.x)
                m_nv->insert(gameObject);
            else
                m_no->insert(gameObject);
        }
    }
}

QuadTre* QuadTre::find(const Vector2d& p)
{
   if (isLeaf()) {
       return this;
   }
   else {
       Vector2d m = (m_a+m_c)/2;
       if (p.y < m.y)
       {
           if (p.x < m.x)
               m_sv->find(p);
           else
               m_so->find(p);
       }
       else {
           if (p.x < m.x)
               m_nv->find(p);
           else
               m_no->find(p);
       }
   }
}

const QuadTre *QuadTre::find(const Vector2d& p) const
{
   if (isLeaf()) {
       return this;
   }
   else {
       Vector2d m = (m_a+m_c)/2;
       if (p.y < m.y)
       {
           if (p.x < m.x)
               m_sv->find(p);
           else
               m_so->find(p);
       }
       else {
           if (p.x < m.x)
               m_nv->find(p);
           else
               m_no->find(p);
       }
   }
}

////////////// eksamen h2021 - oppgave 4a //////////////////
void QuadTre::testdata()
{
    double xmin{-2.0}, xmax{2.0}, ymin{-2.0}, ymax{2.0};
    double xlengde = xmax-xmin;
    double ylengde = ymax-ymin;
    for (auto i=0; i<20; i++)
    {
        auto x = static_cast<double>(std::rand())/RAND_MAX;
        auto y = static_cast<double>(std::rand())/RAND_MAX;
        x = xmin + xlengde*x;
        y = ymin + ylengde*y;
        std::cout << x << " , " << y << std::endl;
        m_v[i] = Vector2d{x,y};
    }
    for (auto i=0; i<20; i++)
    {
        auto x = static_cast<double>(std::rand())/RAND_MAX;
        auto y = static_cast<double>(std::rand())/RAND_MAX;
        x = 2.0*x;
        y = 2.0*y;
        std::cout << x << " , " << y << std::endl;
        m_v[i] = Vector2d{x,y};
    }
    // legger inn punktene som gameobjekter med navn "x"
    for (auto it=m_v.begin(); it!=m_v.end(); it++)
    {
        auto go = GameObject(*it,"x");
        m_gameObjects.push_back(go);
    }

}
////////////// eksamen h2021 - oppgave 4b //////////////////
void QuadTre::split(int n)
{
    if (m_gameObjects.size() > n)
    {
        // opprett subtrær
        auto center = (m_a + m_c)/2;
        Vector2d temp1; temp1.x = center.x; temp1.y = m_a.y;
        Vector2d temp2; temp2.x = m_a.x; temp2.y = center.y;
        m_sv = new QuadTre(m_a, temp1, center, temp2);
        temp2.x = m_b.x;
        m_so = new QuadTre(temp1, m_b, temp2, center);
        temp1.y = m_c.y;
        m_no = new QuadTre(center, temp2, m_c, temp1);
        temp2.x = m_a.x;
        m_nv = new QuadTre(temp2, center, temp1, m_d);
        // kopier punktene/objektene ned til respektive subtrær
        for (auto it=m_gameObjects.begin(); it!=m_gameObjects.end(); it++)
        {
            if (it->m_position.x < center.x) {
                if (it->m_position.y < center.y) {
                    m_sv->m_gameObjects.push_back(*it);
                }
                else {
                    m_nv->m_gameObjects.push_back(*it);
                }
            } else {
                if (it->m_position.y < center.y) {
                    m_so->m_gameObjects.push_back(*it);
                } else {
                    m_no->m_gameObjects.push_back(*it);
                }
            }
        }
        // eventuelt kan punktene slettes i den kallende noden nå,
        // eller vi kan bruke referanser. Dette er ikke optimalt effektiv kode
        // Så gjentas splittingen rekursivt
        m_sv->split(n);
        m_so->split(n);
        m_no->split(n);
        m_nv->split(n);
    }
}
