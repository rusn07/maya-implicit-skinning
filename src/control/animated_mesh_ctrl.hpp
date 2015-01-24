/*
 Implicit skinning
 Copyright (C) 2013 Rodolphe Vaillant, Loic Barthe, Florian Cannezin,
 Gael Guennebaud, Marie Paule Cani, Damien Rohmer, Brian Wyvill,
 Olivier Gourmel

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License 3 as published by
 the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>
 */
#ifndef ANIMATED_MESH_CTRL_HPP__
#define ANIMATED_MESH_CTRL_HPP__

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "vec3_cu.hpp"
#include "mesh.hpp"
#include "animesh_enum.hpp"
#include "transfo.hpp"
#include "bone_type.hpp"
#include "bone.hpp"

// Forward definitions ---------------------------------------------------------
struct Animesh;
struct Skeleton;
// END Forward definitions -----------------------------------------------------




/** @brief Settings Controler for the animated mesh
    This class provide a control interface for the animated mesh.
    Point selection and coloring are handle here.
*/

class Animated_mesh_ctrl {
public:
    Animated_mesh_ctrl(const Mesh *mesh, std::shared_ptr<const Skeleton> skel);
    ~Animated_mesh_ctrl();

    void update_base_potential();
    void get_base_potential(std::vector<float> &pot) const;
    void set_base_potential(const std::vector<float> &pot);

    //--------------------------------------------------------------------------
    /// @name Mesh deformation
    //--------------------------------------------------------------------------

    /// Compute the deformation according to the bone with the current
    /// parameters (dual quat or ssd or implicit skinning etc.)
    void deform_mesh();

    void set_do_smoothing(bool state);
    void set_smooth_factor(int i, float fact);
    void set_nb_iter_smooting(int nb_iter);

    /// Set how the mesh is smoothed
    //@{
    void smooth_conservative();
    void smooth_laplacian();
    void smooth_tangential();
    void smooth_humphrey();
    //@}

    /// Do we smooth the whole mesh or just joints
    void set_local_smoothing(bool state);
    void set_smooth_force_a (float alpha);
    void set_smooth_force_b (float beta);
    void set_smooth_smear   (float val );

    void set_smoothing_weights_diffusion_iter(int nb_iter);

    int  get_nb_iter_smooth(){ return _nb_iter;       }

    // Get the current (possibly deformed) vertices, in their original order.
    void get_anim_vertices_aifo(std::vector<Point_cu>& anim_vert) const;

    // Copy the given vertices into the mesh.
    void copy_vertices(const std::vector<Vec3_cu> &vertices);

    // Return the number of vertices in the mesh.  Calls to copy_vertices must have the
    // same number of vertices.
    int get_nb_vertices() const;

    //--------------------------------------------------------------------------
    /// @name File import/export
    //--------------------------------------------------------------------------

    void load_ism(const char* filename);

private:
#if !defined(NO_CUDA) // XXX: remove this stuff
    //--------------------------------------------------------------------------
    /// @name Tools file import
    //--------------------------------------------------------------------------

    void read_hrbf_env_weights( std::ifstream& file,
                                std::vector<std::vector<float4> >& bone_weights);

    void read_weights(std::ifstream& file,
                      std::vector<float4>& weights );
#endif
    //--------------------------------------------------------------------------
    /// @name Attributes
    //--------------------------------------------------------------------------

    bool _factor_bones;    ///< factor hrbf samples of siblings in a single bone
    int  _nb_iter;         ///< number of iterations for the mesh smoothing

public:
    Animesh* _animesh;
    std::shared_ptr<const Skeleton> skel;
};

#endif // ANIMATED_MESH_CTRL_HPP__
