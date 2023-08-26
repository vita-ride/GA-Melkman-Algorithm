#ifndef MELKMAN_ALGORITHM_H
#define MELKMAN_ALGORITHM_H

#include "algoritambaza.h"
#include <cmath>
#include <deque>

class MelkmanKonveksniOmotac : public AlgoritamBaza
{
public:
    MelkmanKonveksniOmotac(QWidget *pCrtanje,
                    int pauzaKoraka,
                    const bool &naivni = false,
                    std::string imeDatoteke = "",
                    int brojTacaka = BROJ_SLUCAJNIH_OBJEKATA);

    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *painter) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *painter) const final;

    const std::deque<QPoint> &getKonveksniOmotac() const;
    const std::vector<QLine> &getNaivniOmotac() const;


private:
    void naglasiTrenutno(QPainter *painter, unsigned long i, const char *s) const;

    std::vector<QPoint> generisiNasumicniProstPoligon(int brojTacaka = BROJ_SLUCAJNIH_OBJEKATA) const;
    const QPoint nadjiCentar(const std::vector<QPoint>& tacke) const;
    bool clockwise(const QPoint& a, const QPoint& b, const QPoint& c) const;

    std::vector<QPoint> _tacke;
    std::deque<QPoint> _konveksniOmotac;
    bool kraj = false;
    int _povrsina;
    unsigned long _i, _j, _k;
    std::vector<QLine> _naivniOmotac;
};

#endif // MELKMAN_ALGORITHM_H
