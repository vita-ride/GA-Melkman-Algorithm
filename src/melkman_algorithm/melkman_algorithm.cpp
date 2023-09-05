#include "melkman_algorithm.h"
#include "pomocnefunkcije.h"

MelkmanKonveksniOmotac::MelkmanKonveksniOmotac(QWidget *pCrtanje,
                                 int pauzaKoraka,
                                 const bool &naivni,
                                 std::string imeDatoteke,
                                 int brojTacaka)
    : AlgoritamBaza(pCrtanje, pauzaKoraka, naivni)
{
    if (imeDatoteke == "")
        _tacke = generisiNasumicniProstPoligon(brojTacaka);
    else
        _tacke = ucitajPodatkeIzDatoteke(imeDatoteke);
    _k = _tacke.size();
}



void MelkmanKonveksniOmotac::pokreniAlgoritam() {
    kraj = false;
    // Prvo se dodaju prve tri tacke

    // Treca tacka se stavlja i na pocetak
    _konveksniOmotac.push_front(_tacke[2]);

    // Uzima se u obzir da li je dat prost poligon u smeru kazaljke na satu ili obrnuto
    if (clockwise(_tacke[0], _tacke[1], _tacke[2])){
        _konveksniOmotac.push_front(_tacke[0]);
        _konveksniOmotac.push_front(_tacke[1]);
    } else {
        _konveksniOmotac.push_front(_tacke[1]);
        _konveksniOmotac.push_front(_tacke[0]);
    }

    _konveksniOmotac.push_front(_tacke[2]);

    // Front i back su ljubicaste
    // Desno od back je crvena
    // Levo od front je plava

    for (unsigned i = 3; i < _tacke.size(); i++) {

        auto front = _konveksniOmotac.front();
        _konveksniOmotac.pop_front();
        auto blue = _konveksniOmotac.front();
        _konveksniOmotac.push_front(front);

        auto back = _konveksniOmotac.back();
        _konveksniOmotac.pop_back();
        auto red = _konveksniOmotac.back();
        _konveksniOmotac.push_back(back);

        if (clockwise(blue, front, _tacke[i]) && clockwise(back, red, _tacke[i])){
            // samo za crtanje
            _konveksniOmotac.push_back(_tacke[i]);
            _konveksniOmotac.push_front(_tacke[i]);
            AlgoritamBaza_updateCanvasAndBlock()
            _konveksniOmotac.pop_back();
            _konveksniOmotac.pop_front();
            continue;
        }

        while (!clockwise(blue, front, _tacke[i])) {
            _konveksniOmotac.pop_front();
            front = _konveksniOmotac.front();
            _konveksniOmotac.pop_front();
            blue = _konveksniOmotac.front();
            _konveksniOmotac.push_front(front);

            // samo za crtanje
            _konveksniOmotac.push_back(_tacke[i]);
            _konveksniOmotac.push_front(_tacke[i]);
            AlgoritamBaza_updateCanvasAndBlock()
            _konveksniOmotac.pop_back();
            _konveksniOmotac.pop_front();
        }
        _konveksniOmotac.push_front(_tacke[i]);
        back = _konveksniOmotac.back();
        _konveksniOmotac.pop_back();
        red = _konveksniOmotac.back();
        _konveksniOmotac.push_back(back);

        while (!clockwise(back, red, _tacke[i])) {
            _konveksniOmotac.pop_back();
            back = _konveksniOmotac.back();
            _konveksniOmotac.pop_back();
            red = _konveksniOmotac.back();
            _konveksniOmotac.push_back(back);

            // samo za crtanje
            _konveksniOmotac.push_back(_tacke[i]);
            AlgoritamBaza_updateCanvasAndBlock()
            _konveksniOmotac.pop_back();

        }
        _konveksniOmotac.push_back(_tacke[i]);
    }
    kraj = true;
    AlgoritamBaza_updateCanvasAndBlock()
    emit animacijaZavrsila();
}
void MelkmanKonveksniOmotac::crtajAlgoritam(QPainter *painter) const {

    if (!painter) return;

    auto pen = painter->pen();

    pen.setColor(Qt::gray);
    painter->setPen(pen);
    for(auto i = 1ul; i < _tacke.size(); i++) {
       painter->drawLine(_tacke[i-1], _tacke[i]);
    }
    painter->drawLine(_tacke[0], _tacke[_tacke.size()-1]);

    std::vector<QPoint> omotac(_konveksniOmotac.begin(), _konveksniOmotac.end());

    if (omotac.size() > 3) {
        pen.setColor(Qt::yellow);
        painter->setPen(pen);
        for(auto i = 1ul; i < omotac.size() - 1; i++) {
           painter->drawLine(omotac[i-1], omotac[i]);
        }

        if (kraj) {
            painter->drawLine(omotac[omotac.size() - 2], omotac[omotac.size() - 1]);
            pen.setColor(Qt::black);
            painter->setPen(pen);
            for (auto tacka : omotac) {
                painter->drawEllipse(tacka, 6, 6);
            }
        }
        else {
            pen.setColor(Qt::darkYellow);
            painter->setPen(pen);
            painter->drawLine(omotac[omotac.size() - 2], omotac[omotac.size() - 1]);

            pen.setColor(Qt::magenta);
            painter->setPen(pen);
            painter->drawEllipse(omotac[0], 6, 6);

            pen.setColor(Qt::red);
            painter->setPen(pen);
            painter->drawEllipse(omotac[1], 6, 6);

            pen.setColor(Qt::blue);
            painter->setPen(pen);
            painter->drawEllipse(omotac[omotac.size()-2], 6, 6);

            pen.setColor(Qt::black);
            painter->setPen(pen);
            for (auto i = 2; i < (int)omotac.size()-2; i++ ) {
                painter->drawEllipse(omotac[i], 6, 6);
            }
        }


    }
}
void MelkmanKonveksniOmotac::pokreniNaivniAlgoritam() {
    /* Slozenost naivnog algoritma: O(n^3).
     * Prolazi se kroz svaki par tacaka. */
    for (_i = 0; _i < _tacke.size(); _i++) {
        for (_j = 0; _j < _tacke.size(); _j++) {
            if (_i == _j) continue;

            /* Proverava se da li su sve povrsine sa
             * trecom tackom negativne, sto znaci da
             * je trojka negativne orijentacije */
            bool svePovrsineNegativne = true;
            for (_k = 0; _k < _tacke.size(); _k++) {
                if (_k == _i || _k == _j) continue;

                _povrsina = pomocneFunkcije::povrsinaTrougla(_tacke[_i],
                                                             _tacke[_j],
                                                             _tacke[_k]);
                AlgoritamBaza_updateCanvasAndBlock()
                if (_povrsina > 0) {
                    svePovrsineNegativne = false;
                    break;
                }
            }

            /* Ako jesu, dodaje se usmerena duz */
            _k = _tacke.size();
            if (svePovrsineNegativne) {
                _naivniOmotac.emplace_back(_tacke[_i], _tacke[_j]);
                AlgoritamBaza_updateCanvasAndBlock()
            }
        }
    }

    /* Obavestavanje pozivaoca o gotovoj animaciji */
    AlgoritamBaza_updateCanvasAndBlock()
    emit animacijaZavrsila();
}

void MelkmanKonveksniOmotac::naglasiTrenutno(QPainter *painter, unsigned long i, const char *s) const
{
    /* Transformacija cetkice */
    painter->save();
    painter->scale(1, -1);
    painter->translate(0, -2*_tacke[i].y()-10);

    /* Oznacavanje prvog temena */
    painter->drawText(_tacke[i], s);

    /* Ponistavanje transformacija */
    painter->restore();
}

void MelkmanKonveksniOmotac::crtajNaivniAlgoritam(QPainter *painter) const
{
    /* Odustajanje u slucaju greske */
    if (!painter) return;

    /* Dohvatanje olovke */
    auto pen = painter->pen();

    /* Ako je algoritam u toku */
    if (_k < _tacke.size()) {
        /* Podesavanje stila olovke */
        if (_povrsina < 0) {
            pen.setColor(Qt::green);
        } else {
            pen.setColor(Qt::yellow);
        }
        painter->setPen(pen);

        /* Crtanje tekuceg trougla */
        QPainterPath put(_tacke[_i]);
        put.lineTo(_tacke[_j]);
        put.lineTo(_tacke[_k]);
        put.lineTo(_tacke[_i]);
        painter->fillPath(put, pen.color());

        /* Crtanje tekuce stranice */
        pen.setColor(Qt::red);
        painter->setPen(pen);
        painter->drawLine(_tacke[_i], _tacke[_j]);

        /* Podesavanje stila fonta */
        auto font = painter->font();
        font.setWeight(font.Bold);
        font.setPointSizeF(1.3*font.pointSizeF());
        painter->setFont(font);

        /* Podesavanje stila olovke */
        auto olovka = painter->pen();
        olovka.setColor(Qt::darkGreen);
        painter->setPen(olovka);

        /* Naglasavanje tekucih temena */
        naglasiTrenutno(painter, _i, "i");
        naglasiTrenutno(painter, _j, "j");
        naglasiTrenutno(painter, _k, "k");
    }

    /* Podesavanje stila olovke */
    pen.setColor(Qt::red);
    painter->setPen(pen);

    /* Crtanje svih tacaka */
    for(auto &tacka: _tacke) {
        painter->drawPoint(tacka);
    }

    /* Podesavanje stila olovke */
    pen.setColor(Qt::blue);
    painter->setPen(pen);

    /* Crtanje konveksnog omotaca */
    for(auto &duz : _naivniOmotac) {
       painter->drawLine(duz);
    }
}

const std::vector<QLine> &MelkmanKonveksniOmotac::getNaivniOmotac() const
{
    return _naivniOmotac;
}

const std::deque<QPoint> &MelkmanKonveksniOmotac::getKonveksniOmotac() const
{
    return _konveksniOmotac;
}

std::vector<QPoint> MelkmanKonveksniOmotac::generisiNasumicniProstPoligon(int brojTacaka) const {
    auto tacke = AlgoritamBaza::generisiNasumicneTacke(brojTacaka);

    QPoint centar = nadjiCentar(tacke);

    std::sort(tacke.begin(), tacke.end(), [centar](const auto& lhs, const auto& rhs) {
        double ugaoL = std::atan2(lhs.y() - centar.y(), lhs.x() - centar.x());
        double ugaoR = std::atan2(rhs.y() - centar.y(), rhs.x() - centar.x());

        return ugaoL < ugaoR;
    });
    return tacke;
}

const QPoint MelkmanKonveksniOmotac::nadjiCentar(const std::vector<QPoint> &tacke) const {
    int xSuma = 0;
    int ySuma = 0;

    for (const QPoint& tacka : tacke) {
       xSuma += tacka.x();
       ySuma += tacka.y();
    }

    int xProsek = xSuma / tacke.size();
    int yProsek = ySuma / tacke.size();

    return QPoint(xProsek, yProsek);
}

bool MelkmanKonveksniOmotac::clockwise(const QPoint& a, const QPoint& b, const QPoint& c) const {
    return (b.x() - a.x())*(c.y() - a.y()) - (c.x() - a.x())*(b.y() - a.y()) >= 0;
}

