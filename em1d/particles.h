/*
 *  particles.h
 *  zpic
 *
 *  Created by Ricardo Fonseca on 11/8/10.
 *  Copyright 2010 Centro de Física dos Plasmas. All rights reserved.
 *
 */

#ifndef __PARTICLES__
#define __PARTICLES__

#include "zpic.h"
#include "emf.h"
#include "current.h"

#define MAX_SPNAME_LEN 32

typedef struct {
	int ix;
	t_part_data x;
	t_part_data ux, uy, uz;
} t_part;

enum density_type {UNIFORM, STEP, SLAB, RAMP, CUSTOM};

typedef struct {

	float n;				// reference density (defaults to 1.0, multiplies density profile)

	enum density_type type;	// Density profile type
	float start, end;		// Position of the plasma start/end, in simulation units
	
	float ramp[2];     // Initial and final density of the ramp

	float (*custom)(float); // Pointer to custom density function

	unsigned long total_np_inj;	// Total number of particles already injected
	double custom_q_inj;		// Total charge injected (density integral) in custom profile

} t_density;

enum part_boundary { PART_BC_NONE, PART_BC_PERIODIC, PART_BC_OPEN };

typedef struct {
	
	char name[MAX_SPNAME_LEN];
	
	// Particle data buffer
	t_part *part;
	int np;
	int np_max;

	// mass over charge ratio
	t_part_data m_q;

	// total kinetic energy
	double energy;
	
	// charge of individual particle
	t_part_data q;
	
	// Number of particles per cell
	int ppc;

	// Density profile to inject
	t_density density;

	// Initial momentum of particles
	t_part_data ufl[3];
	t_part_data uth[3];

	// Simulation box info
	int nx;
	t_part_data dx;
	t_part_data box;

	// Time step
	float dt;

	// Iteration number
	int iter;

	// Moving window
	int moving_window;
	int n_move;

	// Boundary conditions
	int bc_type;
	
} t_species;

void spec_new( t_species* spec, char name[], const t_part_data m_q, const int ppc, 
			  const t_part_data ufl[], const t_part_data uth[],
			  const int nx, t_part_data box, const float dt, t_density* density );

void spec_move_window( t_species *spec );

void spec_delete( t_species* spec );

void spec_advance( t_species* spec, t_emf* emf, t_current* current );

double spec_time();

/*********************************************************************************************
 
 Diagnostics
 
 *********************************************************************************************/

#define CHARGE 		0x1000
#define PHA    		0x2000
#define PARTICLES   0x3000
#define X1     		0x0001
#define U1     		0x0004
#define U2     		0x0005
#define U3     		0x0006

#define PHASESPACE(a,b) ((a) + (b)*16 + PHA)

void spec_report( const t_species *spec, const int rep_type, 
				  const int pha_nx[], const float pha_range[][2] );


#endif
