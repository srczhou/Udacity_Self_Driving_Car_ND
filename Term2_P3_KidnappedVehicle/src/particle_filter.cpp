/*
 * particle_filter.cpp
 *
 *  Created on: Dec 12, 2016
 *      Author: Tiffany Huang
 */

#include <random>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <math.h> 
#include <iostream>
#include <sstream>
#include <string>
#include <iterator>

#include "particle_filter.h"

using namespace std;

default_random_engine gen;

void ParticleFilter::init(double x, double y, double theta, double std[]) {
	// TODO: Set the number of particles. Initialize all particles to first position (based on estimates of 
	//   x, y, theta and their uncertainties from GPS) and all weights to 1. 
	// Add random Gaussian noise to each particle.
	// NOTE: Consult particle_filter.h for more information about this method (and others in this file).

        num_particles = 200;  //run several experiments to find a good number

        normal_distribution<double> dist_x_init(x, std[0]);
        normal_distribution<double> dist_y_init(y, std[1]);
        normal_distribution<double> dist_theta_init(theta, std[2]);

        for(int i=0; i<num_particles; i++) {
          Particle p;
          p.id = i;
          p.x = dist_x_init(gen);
          p.y = dist_y_init(gen);
          p.theta = dist_theta_init(gen);
          p.weight = 1.0;
          particles.push_back(p);
        }

        is_initialized = true;
}

void ParticleFilter::prediction(double delta_t, double std_pos[], double velocity, double yaw_rate) {
	// TODO: Add measurements to each particle and add random Gaussian noise.
	// NOTE: When adding noise you may find std::normal_distribution and std::default_random_engine useful.
	//  http://en.cppreference.com/w/cpp/numeric/random/normal_distribution
	//  http://www.cplusplus.com/reference/random/default_random_engine/

        normal_distribution<double> dist_x(0, std_pos[0]);
        normal_distribution<double> dist_y(0, std_pos[1]);
        normal_distribution<double> dist_theta(0, std_pos[2]);

        for(int i=0; i<num_particles; i++) {
          if(fabs(yaw_rate) < 0.00001) {
            particles[i].x += velocity * delta_t * cos(particles[i].theta);
            particles[i].y += velocity * delta_t * sin(particles[i].theta);
          }
          else {
            particles[i].x += velocity/yaw_rate * ( sin(particles[i].theta +
                              yaw_rate*delta_t) - sin(particles[i].theta) );
            particles[i].y += velocity/yaw_rate * ( cos(particles[i].theta) -
                              cos(particles[i].theta + yaw_rate*delta_t) );
            particles[i].theta += yaw_rate * delta_t;
          }
          particles[i].x += dist_x(gen);
          particles[i].y += dist_y(gen);
          particles[i].theta += dist_theta(gen);
        }
}

void ParticleFilter::dataAssociation(std::vector<LandmarkObs> predicted, std::vector<LandmarkObs>& observations) {
	// TODO: Find the predicted measurement that is closest to each observed measurement and assign the 
	//   observed measurement to this particular landmark.
	// NOTE: this method will NOT be called by the grading code. But you will probably find it useful to 
	//   implement this method and use it as a helper during the updateWeights phase.

        int map_id = -1;
        for(int i=0; i<observations.size(); i++) {
          double min_dist = 1e20;
          for(int j=0; j<predicted.size(); j++) {
            double tmp_dist = dist(observations[i].x, observations[i].y,
                                   predicted[j].x, predicted[j].y);
            if(tmp_dist < min_dist) {
              min_dist = tmp_dist;
              map_id = predicted[j].id;
            }
          }
          observations[i].id = map_id;
        }
}

void ParticleFilter::updateWeights(double sensor_range, double std_landmark[], 
		const std::vector<LandmarkObs> &observations, const Map &map_landmarks) {
  // TODO: Update the weights of each particle using a mult-variate Gaussian distribution. You can read
  //   more about this distribution here: https://en.wikipedia.org/wiki/Multivariate_normal_distribution
  // NOTE: The observations are given in the VEHICLE'S coordinate system. Your particles are located
  //   according to the MAP'S coordinate system. You will need to transform between the two systems.
  //   Keep in mind that this transformation requires both rotation AND translation (but no scaling).
  //   The following is a good resource for the theory:
  //   https://www.willamette.edu/~gorr/classes/GeneralGraphics/Transforms/transforms2d.htm
  //   and the following is a good resource for the actual equation to implement (look at equation 
  //   3.33
  //   http://planning.cs.uiuc.edu/node99.html

  for(int i=0; i < num_particles; i++) {

    //transform observations from car coordinates to map coordinates
    vector<LandmarkObs> transformed_obs;
    for(int j=0; j<observations.size(); j++) {
      LandmarkObs tmp_obs;
      tmp_obs.x = particles[i].x + cos(particles[i].theta) * observations[j].x
                  - sin(particles[i].theta) * observations[j].y;
      tmp_obs.y = particles[i].y + sin(particles[i].theta) * observations[j].x
                  + cos(particles[i].theta) * observations[j].y;
      tmp_obs.id = observations[j].id;
      transformed_obs.push_back(tmp_obs);
    }

    //choose landmarks within sensor range as candidates of association
    vector<LandmarkObs> predicted_landmark;
    for(int j=0; j<map_landmarks.landmark_list.size(); j++) {
      LandmarkObs tmp_pre;
      tmp_pre.x  = map_landmarks.landmark_list[j].x_f;
      tmp_pre.y  = map_landmarks.landmark_list[j].y_f;
      tmp_pre.id = map_landmarks.landmark_list[j].id_i;
      if( dist(tmp_pre.x, tmp_pre.y, particles[i].x, particles[i].y)
          < sensor_range) {
        predicted_landmark.push_back(tmp_pre);
      }
    }

    //data association
    dataAssociation(predicted_landmark, transformed_obs);

    //calculate the particle's final weight
    particles[i].weight = 1.0;
    double pre_x, pre_y;
    for(int j=0; j<transformed_obs.size(); j++) {
      for(int k=0; k<predicted_landmark.size(); k++) {
        if(predicted_landmark[k].id == transformed_obs[j].id) {
          pre_x = predicted_landmark[k].x;
          pre_y = predicted_landmark[k].y;
          break;
        }
      }
      double gauss_norm = 1/(2*M_PI*std_landmark[0]*std_landmark[1]);
      double exponent = (pow(pre_x - transformed_obs[j].x, 2)
                        / (2 * pow(std_landmark[0], 2)))
                        + (pow(pre_y - transformed_obs[j].y, 2)
                        / (2 * pow(std_landmark[1], 2)));
      particles[i].weight *= gauss_norm * exp(-exponent);
    }
  }
}

void ParticleFilter::resample() {
	// TODO: Resample particles with replacement with probability proportional to their weight. 
	// NOTE: You may find std::discrete_distribution helpful here.
	//   http://en.cppreference.com/w/cpp/numeric/random/discrete_distribution

        vector<Particle> new_particles;

        vector<double> weights;
        for(int i=0; i<num_particles; i++) {
          weights.push_back(particles[i].weight);
        }
        discrete_distribution<int> d(weights.begin(), weights.end());

        for(int i=0; i<num_particles; i++) {
          int idx = d(gen);
          Particle tmp_particle;
          tmp_particle.id     = idx;
          tmp_particle.x      = particles[idx].x;
          tmp_particle.y      = particles[idx].y;
          tmp_particle.theta  = particles[idx].theta;
          tmp_particle.weight = particles[idx].weight;
          new_particles.push_back(tmp_particle);
        }
        particles = new_particles;
}

Particle ParticleFilter::SetAssociations(Particle& particle, const std::vector<int>& associations, 
                                     const std::vector<double>& sense_x, const std::vector<double>& sense_y)
{
    //particle: the particle to assign each listed association, and association's (x,y) world coordinates mapping to
    // associations: The landmark id that goes along with each listed association
    // sense_x: the associations x mapping already converted to world coordinates
    // sense_y: the associations y mapping already converted to world coordinates

    particle.associations= associations;
    particle.sense_x = sense_x;
    particle.sense_y = sense_y;
}

string ParticleFilter::getAssociations(Particle best)
{
	vector<int> v = best.associations;
	stringstream ss;
    copy( v.begin(), v.end(), ostream_iterator<int>(ss, " "));
    string s = ss.str();
    s = s.substr(0, s.length()-1);  // get rid of the trailing space
    return s;
}
string ParticleFilter::getSenseX(Particle best)
{
	vector<double> v = best.sense_x;
	stringstream ss;
    copy( v.begin(), v.end(), ostream_iterator<float>(ss, " "));
    string s = ss.str();
    s = s.substr(0, s.length()-1);  // get rid of the trailing space
    return s;
}
string ParticleFilter::getSenseY(Particle best)
{
	vector<double> v = best.sense_y;
	stringstream ss;
    copy( v.begin(), v.end(), ostream_iterator<float>(ss, " "));
    string s = ss.str();
    s = s.substr(0, s.length()-1);  // get rid of the trailing space
    return s;
}
