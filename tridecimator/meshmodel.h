/****************************************************************************
* MeshLab                                                           o o     *
* A versatile mesh processing toolbox                             o     o   *
*                                                                _   O  _   *
* Copyright(C) 2005                                                \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *   
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/
/****************************************************************************
  History
$Log: not supported by cvs2svn $
Revision 1.45  2008/04/04 10:03:13  cignoni
BIG CHANGE: now also the vertex vector has optional components (mark, and curvature components)
Removed the dangerous 'using namespace' (never put them in .h files!)

Revision 1.44  2008/01/28 13:00:53  cignoni
added delMesh method

Revision 1.43  2007/12/11 17:14:43  cignoni
removed uselss include ocf for vertex

Revision 1.42  2007/10/23 07:50:41  cignoni
added vertex normals to the default of a mesh. Added trBBox function

Revision 1.41  2007/10/02 07:59:32  cignoni
New filter interface. Hopefully more clean and easy to use.

Revision 1.40  2007/09/07 12:01:10  ldpmatic
Avoiding gcc's warning

Revision 1.39  2007/07/24 07:16:51  cignoni
moved matrix inside mesh class and added safe init at construction time

Revision 1.38  2007/07/13 15:16:48  cignoni
Corrected bug on bbox of multiple meshes

Revision 1.37  2007/07/10 07:19:29  cignoni
** Serious Changes **
again on the MeshDocument, the management of multiple meshes, layers, and per mesh transformation

Revision 1.36  2007/06/01 10:04:21  ponchio
!   !=  ~

Revision 1.35  2007/06/01 09:19:24  cignoni
Added MeshDocument and computation of VertexBorderFlags when asking border flags

Revision 1.34  2007/04/16 09:24:37  cignoni
** big change **
Added Layers managemnt.
Interfaces are changing...

Revision 1.33  2007/03/20 16:22:34  cignoni
Big small change in accessing mesh interface. First step toward layers

Revision 1.32  2007/03/14 22:59:34  cignoni
Texture -> TexCoord name change

Revision 1.31  2007/03/03 02:03:26  cignoni
Reformatted lower bar, added number of selected faces. Updated about dialog

Revision 1.30  2007/01/11 10:40:48  cignoni
added a safety test in restorecolor

Revision 1.29  2006/11/29 01:03:38  cignoni
Edited some comments

Revision 1.28  2006/11/07 09:03:40  cignoni
Added clearDataMask

Revision 1.27  2006/10/15 23:45:51  cignoni
Added orderedEdge constructor for gcc compiling of quadric simplifciation stuff

Revision 1.26  2006/10/10 21:16:13  cignoni
Added VF optional component

Revision 1.25  2006/09/22 06:28:02  granzuglia
abstract pointer to fileformat's dependent additional info added

****************************************************************************/

#ifndef MESHMODEL_H
#define MESHMODEL_H


#include <stdio.h>
#include <time.h>

#include <vcg/simplex/vertexplus/base.h>
#include <vcg/simplex/vertexplus/component_ocf.h>
#include <vcg/simplex/edge/edge.h>
#include <vcg/simplex/faceplus/base.h>
#include <vcg/simplex/faceplus/component_ocf.h>
#include <vcg/simplex/face/topology.h>

#include <vcg/complex/trimesh/base.h>
#include <vcg/complex/trimesh/update/topology.h>
#include <vcg/complex/trimesh/update/flag.h>


#include <wrap/callback.h>
#include <wrap/io_trimesh/io_mask.h>
#include <wrap/io_trimesh/additionalinfo.h>


class CEdge;   
class CFaceO;
class CVertexO;

//Vert Mem Occupancy  --- 40b ---

class CVertexO  : public vcg::VertexSimp2< CVertexO, CEdge, CFaceO, 
	vcg::vert::InfoOcf,      /* 4b */
  vcg::vert::Coord3f,     /* 12b */ 
  vcg::vert::BitFlags,    /*  4b */
  vcg::vert::Normal3f,    /* 12b */
  vcg::vert::Qualityf,    /*  4b */
  vcg::vert::Color4b,      /*  4b */
  vcg::vert::VFAdjOcf,    /*  0b */
  vcg::vert::MarkOcf,     /*  0b */
  vcg::vert::CurvaturefOcf, /*  0b */
  vcg::vert::CurvatureDirfOcf  /*  0b */
  >{ 
};

  
class CEdge : public vcg::Edge<CEdge,CVertexO> {
public:
  inline CEdge() {};
  inline CEdge( CVertexO * v0, CVertexO * v1):vcg::Edge<CEdge,CVertexO>(v0,v1){};
  static inline CEdge OrderedEdge(VertexType* v0,VertexType* v1){
   if(v0<v1) return CEdge(v0,v1);
   else return CEdge(v1,v0);
  }

  //inline CEdge( Edge<CEdge,CVertexO> &e):Edge<CEdge,CVertexO>(e){};
};

//Face Mem Occupancy  --- 32b ---

class CFaceO    : public vcg::FaceSimp2<  CVertexO, CEdge, CFaceO,  
      vcg::face::InfoOcf,              /* 4b */
      vcg::face::VertexRef,            /*12b */
      vcg::face::BitFlags,             /* 4b */
      vcg::face::Normal3f,             /*12b */
      vcg::face::MarkOcf,              /* 0b */
      vcg::face::Color4bOcf,           /* 0b */
      vcg::face::FFAdjOcf,             /* 0b */
      vcg::face::VFAdjOcf,             /* 0b */
      vcg::face::WedgeTexCoordfOcf      /* 0b */
    > {};

class CMeshO    : public vcg::tri::TriMesh< vcg::vert::vector_ocf<CVertexO>, vcg::face::vector_ocf<CFaceO> > {
public :
	int sfn; //The number of selected faces.
  vcg::Matrix44f Tr; // Usually it is the identity. It is applied in rendering and filters can or cannot use it. (most of the filter will ignore this)
  const vcg::Box3f &trBB() { 
	static vcg::Box3f bb;
	bb.SetNull();
	bb.Add(Tr,bbox);
		return bb;
	}
};

/*
MeshModel Class
The base class for representing a single mesh.
It contains a single vcg mesh object with some additional information for keeping track of its origin and of what info it has. 
*/
#if 0
class MeshModel : public vcg::tri::io::Mask
{
public:

 	enum FilterReq {  MM_NONE          = 0x0000,
                    MM_BORDERFLAG    = 0x0001,
                    MM_FACETOPO      = 0x0002,
                    MM_WEDGTEXCOORD  = 0x0004,
                    MM_FACECOLOR     = 0x0008,
                    MM_FACEMARK      = 0x0010,
                    MM_VERTMARK      = 0x0020,
                    MM_VERTFACETOPO  = 0x0040,
										MM_CURV          = 0x0080,
										MM_CURVDIR       = 0x0100,
                    MM_ALL           = 0xffff} ;


  CMeshO cm;

public:
  //vcg::GlTrimesh<CMeshO> glw;
  std::vector<vcg::Color4b> originalVertexColor;
	std::string fileName;

  // Bitmask denoting what fields are currently kept updated in mesh
  // it is composed by OR-ing MM_XXXX enums (defined in the above FilterReq)
  int currentDataMask;

  // Bitmask denoting what fields are loaded/saved
  // it is composed by OR-ing IOM_XXXX enums (defined in tri::io::Mask)
  int ioMask;
	
  bool busy;    // used in processing. To disable access to the mesh by the rendering thread
	bool visible; // used in rendering; Needed for toggling on and off the meshes
	
	
  //abstract pointer to fileformat's dependent additional info
  AdditionalInfo* addinfo;

  MeshModel() {    
    glw.m=&cm; 
    currentDataMask=MM_NONE;
    ioMask= IOM_VERTCOORD | IOM_FACEINDEX | IOM_FLAGS | IOM_VERTNORMAL;
    busy=true;
		visible=true;
		cm.Tr.SetIdentity();
		cm.sfn=0;
  }
  bool Render(vcg::GLW::DrawMode dm, vcg::GLW::ColorMode cm, vcg::GLW::TextureMode tm);
  bool RenderSelectedFaces();

  inline void storeVertexColor()
  {
    originalVertexColor.resize(cm.vert.size());
    std::vector<vcg::Color4b>::iterator ci;
	  CMeshO::VertexIterator vi;
	  for(vi=cm.vert.begin(),ci=originalVertexColor.begin();vi!=cm.vert.end();++vi,++ci) 
      (*ci)=(*vi).C();
  }
  inline void restoreVertexColor()
  {
    if(originalVertexColor.empty()) return;
    if(originalVertexColor.size() != cm.vert.size()) return;
    std::vector<vcg::Color4b>::iterator ci;
	  CMeshO::VertexIterator vi;
	  for(vi=cm.vert.begin(),ci=originalVertexColor.begin();vi!=cm.vert.end();++vi,++ci) 
      (*vi).C()=(*ci);
  }
	
// This function is roughly equivalent to the updateDataMask,
// but it takes in input a mask coming from a filetype instead of a filter requirement (like topology etc)
  void Enable(int openingFileMask)
  {
   if( openingFileMask & IOM_WEDGTEXCOORD ) updateDataMask(MM_WEDGTEXCOORD);
   if( openingFileMask & IOM_FACECOLOR    ) updateDataMask(MM_FACECOLOR);
	 
  }

  // Ogni filtro dichiara
  // 1) di che cosa ha bisogno
  // 2) che cosa sa aggiornare (di solito quello di cui ha bisogno)
  // 3) quello che ha cambiato (vertici topologia colore)

  // il framework si preoccupa
  // 1) prima di invocare il filtro di preparare quel che serve
  // 2) dopo il filtro di aggiornare quello che non ha aggiornato il filtro

  // Enable optional fields that could be needed
  void updateDataMask(int neededDataMask)
  {
   if( ( (neededDataMask & MM_FACETOPO)!=0) && (currentDataMask& MM_FACETOPO)==0)			
   {
    cm.face.EnableFFAdjacency();
    currentDataMask |= MM_FACETOPO;
	  vcg::tri::UpdateTopology<CMeshO>::FaceFace(cm);
   }
   if( ( (neededDataMask & MM_VERTFACETOPO)!=0) && (currentDataMask& MM_VERTFACETOPO)==0)			
   {
		cm.vert.EnableVFAdjacency();
    cm.face.EnableVFAdjacency();
    currentDataMask |= MM_VERTFACETOPO;
	  vcg::tri::UpdateTopology<CMeshO>::VertexFace(cm);
   }
   if( ( (neededDataMask & MM_BORDERFLAG)!=0) && (currentDataMask& MM_BORDERFLAG)==0)			
   {
     if(currentDataMask& MM_FACETOPO) vcg::tri::UpdateFlags<CMeshO>::FaceBorderFromFF(cm);
     else vcg::tri::UpdateFlags<CMeshO>::FaceBorderFromNone(cm);
		 vcg::tri::UpdateFlags<CMeshO>::VertexBorderFromFace(cm);

     currentDataMask |= MM_BORDERFLAG;
   }
   if( ( (neededDataMask & MM_WEDGTEXCOORD)!=0) && (currentDataMask& MM_WEDGTEXCOORD)==0)			
   {
    cm.face.EnableWedgeTex();
    currentDataMask |= MM_WEDGTEXCOORD;
   }
   if( ( (neededDataMask & MM_FACECOLOR)!=0) && (currentDataMask& MM_FACECOLOR)==0)			
   {
    cm.face.EnableColor();
    currentDataMask |= MM_FACECOLOR;
   } 
   if( ( (neededDataMask & MM_FACEMARK)!=0) && (currentDataMask& MM_FACEMARK)==0)			
   {
    cm.face.EnableMark();
    currentDataMask |= MM_FACEMARK;
   }
	 if( ( (neededDataMask & MM_VERTMARK)!=0) && (currentDataMask& MM_VERTMARK)==0)			
		{
			cm.vert.EnableMark();
			currentDataMask |= MM_VERTMARK;
		}
		if( ( (neededDataMask & MM_CURV)!=0) && (currentDataMask& MM_CURV)==0)			
		{
			cm.vert.EnableCurvature();
			currentDataMask |= MM_CURV;
		}
		if( ( (neededDataMask & MM_CURVDIR)!=0) && (currentDataMask& MM_CURVDIR)==0)			
		{
			cm.vert.EnableCurvatureDir();
			currentDataMask |= MM_CURVDIR;
		}
  }

   void clearDataMask(int neededDataMask)
   {
     currentDataMask = currentDataMask & (~neededDataMask);
   }
};


class RenderMode
{
public:
  	vcg::GLW::DrawMode	drawMode;
  	vcg::GLW::ColorMode	colorMode;
  	vcg::GLW::TextureMode	textureMode;

    bool lighting;
    bool backFaceCull;
    bool doubleSideLighting;  
    bool fancyLighting;
    bool castShadow;
    bool selectedFaces;
    vcg::Point3f lightDir;
		
		
		RenderMode()
		{
      drawMode	= vcg::GLW::DMFlat;
			colorMode = vcg::GLW::CMNone;
			textureMode = vcg::GLW::TMNone;

			lighting = true;
			backFaceCull = false;
			doubleSideLighting = false;
			fancyLighting = false;
			castShadow = false;
      selectedFaces=false;
    }
};

class MeshDocument
{
public :
MeshDocument()
	{
		currentMesh = NULL;
	}
 
 ~MeshDocument()
		{
			foreach(MeshModel *mmp, meshList)
					delete mmp;
   }
	 
	void setCurrentMesh(unsigned int i)
	{
	  assert(i < (unsigned int)meshList.size());
		currentMesh=meshList.at(i);
	}
	
	MeshModel *mm() {
		return currentMesh;
	}
/// The very important member:
/// The list of MeshModels. 
	
  QList<MeshModel *> meshList;	
	
	int size() const {return meshList.size();}

	void addMesh(MeshModel *mm)
	{
	 meshList.push_back(mm);
	 currentMesh=meshList.back();
	}

  bool delMesh(MeshModel *mmToDel)
	{
		if(meshList.size()==1) return false; 
		QMutableListIterator<MeshModel *> i(meshList);
		while (i.hasNext()) {
     MeshModel *md = i.next();
     if (md==mmToDel) 
		  {
         i.remove();
				 delete mmToDel;
			}
		}
		if(currentMesh == mmToDel) setCurrentMesh(0);
		return true;
 }
		
  int vn() /// Sum of all the vertices of all the meshes
	{ 
			int tot=0;
			foreach(MeshModel *mmp, meshList)
					tot+= mmp->cm.vn;
			return tot;
	}
	int fn() { 
			int tot=0;
			foreach(MeshModel *mmp, meshList)
					tot+= mmp->cm.fn;
			return tot;
	}		

 vcg::Box3f bbox()
 {
		vcg::Box3f FullBBox;
		foreach(MeshModel * mp, meshList) 
			FullBBox.Add(mp->cm.Tr,mp->cm.bbox);
		return FullBBox;
 }
	private:
		MeshModel *currentMesh;	
};
#endif
#endif