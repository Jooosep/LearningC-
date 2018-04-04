#include "Param.h"

Param::Param(int _nb, vector<Particle> *_particles, float *_dt, bool *_LMB, Vector2f *_mousePos)
{
	nb = _nb;
	particles = _particles;
	dt = _dt;
	LMB = _LMB;
	mousePos = _mousePos;
}

