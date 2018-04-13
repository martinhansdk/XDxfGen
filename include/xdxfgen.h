/*
 * XDxfGen.h
 * version 0.3.1 (28 Sep 2014)
 * C++ library to support simple drawing export to Autodesk DXF format
 * License: LGPL or MIT. Static linking is allowed.
 
 * copyright (c) 2011, fluxrobotics
 * copyright (c) 2014, open source community

 * Library style:    single C++ header only.
 * Dependencies:     only standard C++ STL.
 * Platforms:        Any suitable.
 * Report Bugs: to Git or e-mail

 * Where to test output files:
        Test dxf output online at: http://www.sharecad.org/Default.aspx
        Also: QCad, 3d studio Max, Autocad, VoloView (?, failed, discontinued)

 * Similar libraries:
        QCad dxfilb: http://www.qcad.org/dxflib.html - mainly for parsing dxf

 * Contribute to Git repository: https://github.com/okosan/XDxfGen
 * E-Mail support: info@endrobene.org

 * Web site: wiki.endrobene.org, fluxrobotics.blogspot.com

 * This is an extended mod of [nirvanarupali] "Creating AutoCad DXF in C++ ":
    http://www.dreamincode.net/forums/topic/37974-creating-autocad-dxf-in-c/
 */

#ifndef H_XDXFGEN_H
#define H_XDXFGEN_H


#include <fstream>
#include <iostream>
#include <string>
#include <sstream>  // To avoid bug when user enters letters or words.
#include <vector>

using namespace std;

// uncomment this if you want to use not yet finished routines
// or routines under the test
#define X_DXFGEN_NOT_IMPLEMENTED

#ifndef XDxfGen_X_PI
#define XDxfGen_X_PI
const double X_PI = 3.1415926535897932384626433832795;
#endif

template<typename T>
class XDxfGen
{
public:
    XDxfGen();
    ~XDxfGen();

    void begin(const std::string &filename);
    bool end();

    // Put a circle on an output plot
    bool circle(T radius, T x, T y, T width = 0, int layer = 0);
    
    bool line(T x1,T y1,T x2,T y2, T width = 0, int layer = 0);

    bool polyline(const std::vector<T> &vx, const std::vector<T> &vy,
                  T width = 0, int layer = 0);

    bool arc(T center_x, T center_y,
             T radius,
             T start_angle_deg, T end_angle_deg,
             T width = 0, int layer = 0);

    bool point(T x, T y, int layer = 0);

    #ifdef X_DXFGEN_NOT_IMPLEMENTED
    // Not implemented funcs
    // Draw a polyline
    bool lwpolyline(const std::vector<T> &vx, const std::vector<T> &vy,
                    T width = 0, int layer = 0);
    
    // Put an ellipse on an output plot
    bool ellipse(T center_x, T center_y,
        T major_axis_shift_x, T major_axis_shift_y,
        T ratio, T start_angle = 0.0f, T end_angle = 2*X_PI,
        T width = 0, int layer = 0);
    #endif

private:
    ofstream m_file;
    bool f_closed;
    bool m_status;

};


template<typename T>
XDxfGen<T>::XDxfGen()
{
   f_closed = true;
   m_status = true;
}

template<typename T>
XDxfGen<T>::~XDxfGen()
{
   end(); // force close
}

template<typename T>
void XDxfGen<T>::begin(const std::string &filename)
{
    if (!f_closed)
        return;

    m_file.open(filename.c_str(),ios::out);
    f_closed = false;
    m_status = true;

    // === Header section of every dxf file. 
    m_file << 0 << endl;
    m_file << "SECTION" << endl;
    m_file << 2 << endl;
    m_file << "HEADER" << endl;
    m_file << 9 << endl;
    m_file << "$ACADVER" << endl;
    m_file << 1 << endl;
    m_file << "AC1009" << endl; // R11 and R12
    
    // Set Base command (in WCS)
    m_file << 9 << endl;
    m_file << "$INSBASE" << endl;
    m_file << 10 << endl;
    m_file << 0.0 << endl;
    m_file << 20 << endl;
    m_file << 0.0 << endl;
    m_file << 30 << endl;
    m_file << 0.0 << endl;

    /*
    // Drawing lower left corner
    m_file << 9 << endl;
    m_file << "$EXTMIN" << endl;
    m_file << 10 << endl;
    m_file << 0.0 << endl;
    m_file << 20 << endl;
    m_file << 0.0 << endl;

    // Drawing upper right corner
    m_file << 9 << endl;
    m_file << "$EXTMAX" << endl;
    m_file << 10 << endl;
    m_file << 10000.0 << endl;
    m_file << 20 << endl;
    m_file << 10000.0 << endl;
    */
    
    m_file << 0 << endl;
    m_file << "ENDSEC" << endl; 
    // === End of header section

    // Entities section
    m_file << 0 << endl;
    m_file << "SECTION" << endl;
    m_file << 2 << endl;
    m_file << "ENTITIES" << endl;
}

template<typename T>
bool XDxfGen<T>::end()
{
    if (!f_closed)
    {
        // end of sequence objects of dxf file.
        m_file << 0 << endl;
        m_file << "ENDSEC" << endl;
        m_file << 0 << endl;
        m_file << "EOF";

        m_file.close();
        f_closed = true;
    }
    return m_status;
}

template<typename T>
bool XDxfGen<T>::point(T x, T y, int layer)
{
    m_file << 0 << endl; // entity type
    m_file << "POINT" << endl;

    m_file << 8 << endl; // layer name
    m_file << layer << endl;
        
    m_file << 10 << endl;
    m_file << x << endl;
        
    m_file << 20 << endl;
    m_file << y << endl;
        
    m_file << 30 << endl;
    m_file << 0.0 << endl;

    return true;
}

template<typename T>
bool XDxfGen<T>::circle(T radius, T x, T y, T width, int layer)
{
    // Draw the circle
    m_file << 0    << endl;
    m_file << "CIRCLE" << endl;

    m_file << 8 << endl;
    m_file << layer    << endl;    // Layer number (default layer in autocad)

    m_file << 39 << endl;
    m_file << width    << endl;    // Line thickness

    m_file << 10 << endl;    // XYZ is the Center point of circle
    m_file << x << endl;    // X in UCS (User Coordinate System)coordinates

    m_file << 20 << endl;
    m_file << y << endl;    // Y in UCS (User Coordinate System)coordinates

    m_file << 30 << endl;
    m_file << 0.0 << endl;    // Z in UCS (User Coordinate System)coordinates

    m_file << 40 << endl;
    m_file << radius << endl;    // radius of circle

    return true;
}

template<typename T>
bool XDxfGen<T>::line(T x1,T y1,T x2,T y2, T width, int layer)
{
    // Draw the circle
    m_file << 0          << endl;
    m_file << "LINE"  << endl;
    m_file << 8          << endl;
    m_file << layer          << endl;    // Layer number (default layer in autocad)

    m_file << 39 << endl;
    m_file << width    << endl;    // Line thickness

    T z1 = 0.0f;
    T z2 = 0.0f;

    m_file << 10    << endl;    // XYZ is the Center point of circle
    m_file << x1    << endl;    // X in UCS (User Coordinate System)coordinates
    m_file << 20    << endl;
    m_file << y1    << endl;    // Y in UCS (User Coordinate System)coordinates
    m_file << 30    << endl;
    m_file << z1    << endl;    // Z in UCS (User Coordinate System)coordinates

    m_file << 11    << endl;    // XYZ is the Center point of circle
    m_file << x2    << endl;    // X in UCS (User Coordinate System)coordinates
    m_file << 21    << endl;
    m_file << y2    << endl;    // Y in UCS (User Coordinate System)coordinates
    m_file << 31    << endl;
    m_file << z2    << endl;    // Z in UCS (User Coordinate System)coordinates

    return true;
}

template <typename T>
bool XDxfGen<T>::polyline(const std::vector<T> &vx, const std::vector<T> &vy,
                       T width, int layer)
{
    if (vx.size() != vy.size())
        return false;
    int count = vx.size();
    if (!count)
        return false;

    m_file << 0 << endl;
    m_file << "POLYLINE"  << endl;

    m_file << 8 << endl;
    m_file << layer << endl;    // Layer number (default layer in autocad)

    m_file << 39 << endl;
    m_file << width    << endl;    // Line thickness

    m_file << 100 << endl;    // Subclass marker (AcDb2dPolyline or AcDb3dPolyline)
    m_file << "AcDb2dPolyline" << endl;

    m_file << 66 << endl; // Obsolete; formerly an “entities follow flag” (optional; ignore if present)
    m_file << 1 << endl;

    // DXF X,Y always 0
    m_file << 10 << endl;
    m_file << 0.0f << endl;

    m_file << 20 << endl;
    m_file << 0.0f << endl;

    // DXF Polyline elevation
    m_file << 30 << endl;
    m_file << 0.0f << endl;

    m_file << 70 << endl; // Polyline flags
    m_file << 1 << endl; // closed polyline

    // Per-vertex info
    for (int i = 0; i < count; ++i)
    {
        m_file << 0 << endl;
        m_file << "VERTEX"  << endl;

        /*
        5 // Handle
        FD

        330 // Soft-pointer ID/handle to owner dictionary (optional)
        FC

        100 // Subclass marker (AcDbEntity)
        AcDbEntity
        */


        m_file << 8 << endl;
        m_file << layer << endl;    // Layer number (default layer in autocad)

        /*
        m_file << 62 << endl;    // Color number
        m_file << 196 << endl;

        m_file << 420 << endl;    // 24 bit color value
        m_file << 3737736 << endl;
        */

        m_file << 100 << endl;    // Subclass marker (AcDb2dVertex)
        m_file << "AcDb2dVertex" << endl;

        m_file << 10 << endl;
        m_file << vx[i] << endl;

        m_file << 20 << endl;
        m_file << vy[i] << endl;

        m_file << 30 << endl;
        m_file << 0 << endl;
    }

    m_file << 0 << endl; // Sequence end marker
    m_file << "SEQEND" << endl;

    return true;
}

template<typename T>
bool XDxfGen<T>::arc(T center_x, T center_y,
                  T radius,
                  T start_angle_deg, T end_angle_deg,
                  T width, int layer)
{
    // Draw an arc
    m_file << 0    << endl;
    m_file << "ARC" << endl;

    // General entity params
    m_file << 100 << endl;
    m_file << "AcDbEntity" << endl;

    m_file << 8 << endl;
    m_file << layer    << endl;    // Layer number (default layer in autocad)

    // Circle subclass
    m_file << 100 << endl;
    m_file << "AcDbCircle" << endl;

    m_file << 39 << endl;
    m_file << width    << endl;    // Line thickness

    m_file << 10 << endl;    // XYZ is the Center point of an arc
    m_file << center_x << endl;    // X in UCS (User Coordinate System)coordinates

    m_file << 20 << endl;
    m_file << center_y << endl;    // Y in UCS (User Coordinate System)coordinates

    m_file << 30 << endl;
    m_file << 0.0 << endl;    // Z in UCS (User Coordinate System)coordinates

    m_file << 40 << endl;
    m_file << radius << endl;    // arc radius

    // Arc subclass params
    m_file << 100 << endl;
    m_file << "AcDbArc" << endl;

    // Start and end angles
    m_file << 50 << endl;
    m_file << start_angle_deg << endl;

    m_file << 51 << endl;
    m_file << end_angle_deg << endl;

    return true;
}

#ifdef X_DXFGEN_NOT_IMPLEMENTED

template<typename T>
bool XDxfGen<T>::ellipse(T center_x, T center_y, T major_x, T major_y,
        T ratio, T start_angle, T end_angle, T width, int layer)
{
    T center_z = 0.0;
    T major_z = 0.0;

    // Draw the ellipse
    m_file << 0    << endl;
    m_file << "ELLIPSE" << endl; // Entity name

    m_file << 100 << endl;
    m_file << "AcDbEntity" << endl;

    m_file << 8 << endl;
    m_file << layer    << endl;    // Layer number (default layer in autocad)

    m_file << 100 << endl;    // Subclass marker (AcDbEllipse)
    m_file << "AcDbEllipse" << endl;

    // Ellipse center
    m_file << 10 << endl;
    m_file << center_x << endl;    // X in UCS (User Coordinate System) coordinates

    m_file << 20 << endl;
    m_file << center_y << endl;    // Y in UCS (User Coordinate System) coordinates

    m_file << 30 << endl;
    m_file << center_z << endl;    // Z in UCS (User Coordinate System) coordinates


    // Endpoint of major axis
    m_file << 11 << endl;
    m_file << major_x << endl;

    m_file << 21 << endl;
    m_file << major_y << endl;

    m_file << 31 << endl;
    m_file << major_z << endl;

    /*
    // Optional extrusion direction
    m_file << 210 << endl;
    m_file << 0.0 << endl;

    m_file << 220 << endl;
    m_file << 0.0 << endl;

    m_file << 230 << endl;
    m_file << 1.0 << endl;
    */



    // Axis ratio
    m_file << 40 << endl;
    //m_file << ratio << endl;    // Ratio of minor axis to major axis
    m_file << 0.5 << endl;    // Ratio of minor axis to major axis

    // Start and end angle
    m_file << 41 << endl;
    //m_file << start_angle << endl;
    m_file << 0.0 << endl;

    m_file << 42 << endl;
    //    m_file << end_angle << endl;
    m_file << 6.283185307179585 << endl;

    return true;
}

template<typename T>
bool XDxfGen<T>::lwpolyline(const std::vector<T> &vx, const std::vector<T> &vy,
                         T width, int layer)
{
    m_file << 0 << endl;
    m_file << "LWPOLYLINE"  << endl;

    m_file << 8 << endl;
    m_file << layer << endl;    // Layer number (default layer in autocad)

    size_t count = vx.size();
    count = 0;

    m_file << 90 << endl; // number of vertices
    m_file << count << endl;

    /*
    m_file << 43 << endl; // Constant width
    m_file << 0 << endl;

    m_file << 39 << endl; // Thickness
    m_file << 0 << endl;

    m_file << 70 << endl; // Flags
    m_file << 1 << endl; // 1 - closed polyline
    */

    // coordinates
    if (0)
    for (size_t i = 0; i < count; ++i)
    {
        //m_file << 91 << endl;
        //m_file << i << endl; // vertex index

        m_file << 10 << endl;
        m_file << vx[i] << endl;

        m_file << 20 << endl;
        m_file << vy[i] << endl;
    }

    return true;
}
#endif // NON implemented routines yet

#endif
