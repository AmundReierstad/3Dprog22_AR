// ADS101 Algoritmer og Datastrukturer for spill 2020
// Dag Nylund, Høgskolen i Innlandet
// Quadtre - noe er gjort om fra Rectangle eksempel
#ifndef QUADTRE_H
#define QUADTRE_H

#include <vector>
#include <string>

// Hjelpeklasse med operatoroverlasting for + og /
struct Vector2d
{
    double x;
    double y;
    Vector2d operator + (const Vector2d& v) const
    {
            Vector2d u;
            u.x = x + v.x;
            u.y = y + v.y;
            return u;
    }
    Vector2d operator / (int d)
    {
        Vector2d u;
        u.x = x/d;
        u.y = y/d;
        return u;
    }
};

// Denne klassen skal erstattes av et visuelt objekt i 3D
struct GameObject
{
    Vector2d m_position;
    std::string m_navn;
    GameObject() { /* to do */ }
    GameObject(const Vector2d& position, std::string navn)
        : m_position{position}, m_navn{navn} { }
    Vector2d getPosition() const { return m_position; }
};

// Dette er hovedklassen, med litt fra Rectangle og litt fra
// binærtreklassen
class QuadTre
{
private:
    // Punkter som definerer hjørnene til objektet
    // i samme rekkefølge som pekerne til subtrær
    Vector2d m_a;
    Vector2d m_b;
    Vector2d m_c;
    Vector2d m_d;
    // Pekere til subtrær er nødvendig i en dynamisk
    // quadtre implementering. Vi kunne ha lagt det
    // i array også
    QuadTre* m_sv;
    QuadTre* m_so;
    QuadTre* m_no;
    QuadTre* m_nv;

    // Hvert quad lagrer sine egne objekter
    // (vi bør endre til pekere eller referanser)
    std::vector<GameObject> m_gameObjects;
    std::vector<Vector2d> m_v;
    bool isLeaf() const;

public:
    QuadTre();
    QuadTre(const Vector2d &v1, const Vector2d &v2, const Vector2d &v3, const Vector2d & v4);
    void subDivide(int n);                          // Se rectangle eksemplet
    void print() const;
    QuadTre* insert(const GameObject& gameObject);  // Se binærtre eksemplet
    QuadTre* find(const Vector2d& p);               // Se binærtre eksemplet
    const QuadTre* find(const Vector2d& p) const;

    void testdata();
    void split(int n);
};
#endif // QUADTRE_H
