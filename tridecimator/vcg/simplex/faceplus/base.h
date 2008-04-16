/****************************************************************************
* VCGLib                                                            o o     *
* Visual and Computer Graphics Library                            o     o   *
*                                                                _   O  _   *
* Copyright(C) 2004                                                \/)\/    *
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
Revision 1.14  2008/02/04 21:26:44  ganovelli
added ImportLocal which imports all local attributes into vertexplus and faceplus.
A local attribute is everything (N(), C(), Q()....) except pointers to other simplices
(i.e. FFAdj, VFAdj, VertexRef) which are set to NULL.
Added some function for const attributes

Revision 1.13  2008/02/03 23:49:42  cignoni
Important Change. Now GetBBox return a null bbox if called on a deleted face (instead of crashing)

Revision 1.12  2007/05/04 16:40:11  ganovelli
changes to comply "plus" types

Revision 1.11  2007/03/27 09:22:11  cignoni
Added Visited flags

Revision 1.10  2007/03/12 15:37:19  tarini
Texture coord name change!  "TCoord" and "Texture" are BAD. "TexCoord" is GOOD.

Revision 1.9  2007/02/12 19:01:23  ganovelli
added Name(std:vector<std::string>& n) that fills n with the names of the attribute of the face type

Revision 1.8  2006/10/09 20:20:18  cignoni
Increased the maximum number of possible template args from 8  to 9

Revision 1.7  2006/02/27 17:58:11  ponchio
Added some documentation

Revision 1.6  2005/12/16 13:28:09  cignoni
Increased the maximum number of possible template args from 7  to 8

Revision 1.5  2005/12/02 00:41:38  cignoni
Added and removed typenames for gcc compiling.
Added this-> qualifier for referencing the elemntes of the templated base class
(e.g. to refer the possibly overridden flags())  it seems to be needed by the standard

Revision 1.4  2005/11/16 22:56:32  cignoni
Added EmptyMark to base class
Standardized name of flags. It is plural becouse each simplex has many flag.

Revision 1.3  2005/11/12 18:42:18  cignoni
Added ClearS and GetBBox

Revision 1.2  2005/10/14 13:26:57  cignoni
First Really Working version

Revision 1.2  2004/04/03 13:33:55  cignoni
Missing include

Revision 1.1  2004/03/29 08:36:26  cignoni
First working version!


****************************************************************************/
#ifndef __VCG_FACE_PLUS
#define __VCG_FACE_PLUS

#include <vcg/space/point3.h>
#include <vcg/space/texcoord2.h>
#include <vcg/space/color4.h>
#include <vcg/simplex/faceplus/component.h>

namespace vcg {

/*------------------------------------------------------------------*/ 
/* 
The base class of all the recusive definition chain. It is just a container of the typenames of the various simplexes.
These typenames must be known form all the derived classes.
*/

template <class BVT, class BET, class BFT, class BTT>
class FaceTypeHolder{
  public:
  typedef BVT VertexType;
  typedef typename VertexType::CoordType CoordType;
  typedef typename VertexType::ScalarType ScalarType;
  typedef BET EdgeType;
  typedef BFT FaceType;
  typedef BTT TetraType;
  typedef BVT *VertPointer;
  typedef BET *EdgePointer;
  typedef BFT *FacePointer;
  typedef BTT *TetraPointer;
	template <class LeftF>
	void ImportLocal(const LeftF & l){}
  static void Name(std::vector<std::string> & name){}


 // prot
 
};

/* The base class form which we start to add our components.
it has the empty definition for all the standard members (coords, color flags)
Note:
in order to avoid both virtual classes and ambiguous definitions all 
the subsequent overrides must be done in a sequence of derivation.

In other words we cannot derive and add in a single derivation step 
(with multiple ancestor), both the real (non-empty) normal and color but 
we have to build the type a step a time (deriving from a single ancestor at a time). 


*/ 
template <class BVT, class BET=DumET, class BFT=DumFT, class BTT=DumTT>
class FaceBase: public   face::EmptyVertexRef<
                         face::EmptyAdj<
                         face::EmptyColorQuality<
                         face::EmptyNormal<
                         face::EmptyBitFlags<
                         face::EmptyMark<
                         face::EmptyWedgeTexCoord<
                         FaceTypeHolder <BVT, BET, BFT, BTT> > > > > > > >{
};



// Metaprogramming Core

template <class BVT, class BET, class BFT,class BTT,
          template <typename> class A> 
          class FaceArity1: public A<FaceBase<BVT,BET,BFT,BTT> > {};

template <class BVT, class BET, typename BFT, class BTT,
          template <typename> class A, template <typename> class B> 
          class FaceArity2: public B<FaceArity1<BVT,BET,BFT,BTT, A> > {};

template <class BVT, class BET, typename BFT,class BTT,
          template <typename> class A, template <typename> class B, 
          template <typename> class C > 
          class FaceArity3: public C<FaceArity2<BVT,BET,BFT,BTT, A, B> > {};

template <class BVT, class BET, typename BFT,class BTT,
          template <typename> class A, template <typename> class B, 
          template <typename> class C, template <typename> class D> 
          class FaceArity4: public D<FaceArity3<BVT,BET,BFT,BTT, A, B, C> > {};

template <class BVT, class BET, typename BFT,class BTT,
          template <typename> class A, template <typename> class B, 
          template <typename> class C, template <typename> class D,
          template <typename> class E > 
          class FaceArity5: public E<FaceArity4<BVT,BET,BFT,BTT, A, B, C, D> > {};

template <class BVT, class BET, typename BFT,class BTT,
          template <typename> class A, template <typename> class B, 
          template <typename> class C, template <typename> class D,
          template <typename> class E, template <typename> class F > 
          class FaceArity6: public F<FaceArity5<BVT,BET,BFT,BTT, A, B, C, D, E> > {};

template <class BVT, class BET, typename BFT,class BTT,
          template <typename> class A, template <typename> class B, 
          template <typename> class C, template <typename> class D,
          template <typename> class E, template <typename> class F, 
          template <typename> class G  > 
          class FaceArity7: public G<FaceArity6<BVT,BET,BFT,BTT, A, B, C, D, E, F> > {};

template <class BVT, class BET, typename BFT,class BTT,
          template <typename> class A, template <typename> class B, 
          template <typename> class C, template <typename> class D,
          template <typename> class E, template <typename> class F, 
          template <typename> class G, template <typename> class H  > 
          class FaceArity8: public H<FaceArity7<BVT,BET,BFT,BTT, A, B, C, D, E, F, G> > {};

/* The Real Big Face class;

The class __FaceArityMax__ is the one that is the Last to be derived,
and therefore is the only one to know the real members 
(after the many overrides) so all the functions with common behaviour 
using the members defined in the various Empty/nonEmpty component classes 
MUST be defined here. 

I.e. IsD() that uses the overridden Flags() member must be defined here.

*/

template <class BVT, class BET, typename BFT,class BTT,
          template <typename> class A, template <typename> class B, 
          template <typename> class C, template <typename> class D, 
          template <typename> class E, template <typename> class F,
          template <typename> class G, template <typename> class H,
          template <typename> class I  > 
          class FaceArityMax: public I<FaceArity8<BVT,BET,BFT,BTT, A, B, C, D, E, F, G, H> > {

// ----- Flags stuff -----
public:
  
	inline int & UberFlags ()
	{ 
			return this->Flags();
	}
	inline const int UberFlags() const
	{
		return this->Flags();
	}
   	enum { 
		
		DELETED     = 0x00000001,		// Face is deleted from the mesh
		NOTREAD     = 0x00000002,		// Face of the mesh is not readable
		NOTWRITE    = 0x00000004,		// Face of the mesh is not writable
    VISITED     = 0x00000010,		// Face has been visited. Usualy this is a per-algorithm used bit. 
		SELECTED    = 0x00000020,		// Face is selected. Algorithms should try to work only on selected face (if explicitly requested)
		// Border _flags, it is assumed that BORDERi = BORDER0<<i 
		BORDER0     = 0x00000040,
		BORDER1     = 0x00000080,
		BORDER2     = 0x00000100,
		// Face Orientation Flags, used efficiently compute point face distance  
		NORMX				= 0x00000200,
		NORMY				= 0x00000400,
		NORMZ				= 0x00000800,
		// Crease _flags,  it is assumed that FEATUREi = FEATURE0<<i 
		FEATURE0    = 0x00008000,
		FEATURE1    = 0x00010000,
		FEATURE2    = 0x00020000,
		// First user bit
		USER0       = 0x00040000
			};

 
 	///  checks if the Face is deleted
	bool IsD() const {return (this->Flags() & DELETED) != 0;}
	///  checks if the Face is readable
	bool IsR() const {return (this->Flags() & NOTREAD) == 0;}
	///  checks if the Face is modifiable
	bool IsW() const {return (this->Flags() & NOTWRITE)== 0;}
	/// This funcion checks whether the Face is both readable and modifiable
	bool IsRW() const {return (this->Flags() & (NOTREAD | NOTWRITE)) == 0;}
	///  checks if the Face is Modified
	bool IsS() const {return (this->Flags() & SELECTED) != 0;}
	///  checks if the Face is Modified
	bool IsV() const {return (this->Flags() & VISITED) != 0;}
	
	/** Set the flag value
		@param flagp Valore da inserire nel flag
	*/
	void SetFlags(int flagp) {this->Flags()=flagp;}

	/** Set the flag value
		@param flagp Valore da inserire nel flag
	*/
	void ClearFlags() {this->Flags()=0;}

	///  deletes the Face from the mesh
	void SetD() {this->Flags() |=DELETED;}
	///  un-delete a Face
	void ClearD() {this->Flags() &=(~DELETED);}
	///  marks the Face as readable
	void SetR() {this->Flags() &=(~NOTREAD);}
	///  marks the Face as not readable
	void ClearR() {this->Flags() |=NOTREAD;}
	///  marks the Face as writable
	void SetW() {this->Flags() &=(~NOTWRITE);}
	///  marks the Face as notwritable
	void ClearW() {this->Flags() |=NOTWRITE;}
	///  select the Face
	void SetS()		{this->Flags() |=SELECTED;}
	/// Un-select a Face
  void ClearS()	{this->Flags() &= ~SELECTED;}
	///  select the Face
	void SetV()		{this->Flags() |=VISITED;}
	/// Un-select a Face
  void ClearV()	{this->Flags() &= ~VISITED;}
	
	/// This function checks if the face is selected
	bool IsB(int i) const {return (this->Flags() & (BORDER0<<i)) != 0;}
	/// This function select the face
  void SetB(int i)		{this->Flags() |=(BORDER0<<i);}
	/// This funcion execute the inverse operation of SetS()
	void ClearB(int i)	{this->Flags() &= (~(BORDER0<<i));}
	
///  Return the first bit that is not still used
static int &LastBitFlag()
		{
			static int b =USER0;
			return b;
		}

    /// allocate a bit among the flags that can be used by user.
    static inline int NewBitFlag()
		    {
			    LastBitFlag()=LastBitFlag()<<1;
			    return LastBitFlag();
		    }
    // de-allocate a bit among the flags that can be used by user.
    static inline bool DeleteBitFlag(int bitval)
		{	
			if(LastBitFlag()==bitval) {
					LastBitFlag()= LastBitFlag()>>1;
					return true;
			}
			assert(0);
			return false;
		}
	/// This function checks if the given user bit is true
	bool IsUserBit(int userBit){return (this->Flags() & userBit) != 0;}
	/// This function set  the given user bit 
	void SetUserBit(int userBit){this->Flags() |=userBit;}
	/// This function clear the given user bit 
	void ClearUserBit(int userBit){this->Flags() &= (~userBit);}

 template<class BoxType>
  void GetBBox( BoxType & bb ) const
  {
	  if(this->IsD()) 
			{
				bb.SetNull();
				return;
			}
	  bb.Set(this->P(0));
	  bb.Add(this->P(1));
	  bb.Add(this->P(2));
  }


};

template < typename T=int>
class FaceDefaultDeriver : public T {};
          
/*

These are the three main classes that are used by the library user to define its own Facees.
The user MUST specify the names of all the type involved in a generic complex.
so for example when defining a Face of a trimesh you must know the name of the type of the edge and of the face.
Typical usage example:

A Face with coords, flags and normal for use in a standard trimesh:

class MyFaceNf   : public FaceSimp2< VertProto, EdgeProto, MyFaceNf, face::Flag, face::Normal3f  > {};


A Face with coords, and normal for use in a tetrahedral mesh AND in a standard trimesh:

class TetraFace   : public FaceSimp3< VertProto, EdgeProto, TetraFace, TetraProto, face::Coord3d, face::Normal3f  > {};


A summary of the components that can be added to a face (see components.h for details):
          
VertexRef
NormalFromVert, WedgeNormal
Normal3s, Normal3f, Normal3d
WedgeTexCoord2s, WedgeTexCoord2f, WedgeTexCoord2d
BitFlags
WedgeColor, Color4b
Qualitys, Qualityf, Qualityd
Mark                                            //Incremental mark (int)
VFAdj                                           //Topology vertex face adjacency
                                                 (pointers to next face in the ring of the vertex
FFAdj                                           //topology: face face adj
                                                  pointers to adjacent faces

*/

template <class BVT, class BET, class BFT, class BTT,
          template <typename> class A = FaceDefaultDeriver, template <typename> class B = FaceDefaultDeriver,
          template <typename> class C = FaceDefaultDeriver, template <typename> class D = FaceDefaultDeriver,
          template <typename> class E = FaceDefaultDeriver, template <typename> class F = FaceDefaultDeriver,
          template <typename> class G = FaceDefaultDeriver, template <typename> class H = FaceDefaultDeriver,
          template <typename> class I = FaceDefaultDeriver > 
              class FaceSimp3: public FaceArityMax<BVT,BET,BFT,BTT, A, B, C, D, E, F, G, H, I>  {};
class DumTT;
template <class BVT, class BET, class BFT, 
          template <typename> class A = FaceDefaultDeriver, template <typename> class B = FaceDefaultDeriver,
          template <typename> class C = FaceDefaultDeriver, template <typename> class D = FaceDefaultDeriver,
          template <typename> class E = FaceDefaultDeriver, template <typename> class F = FaceDefaultDeriver,
          template <typename> class G = FaceDefaultDeriver, template <typename> class H = FaceDefaultDeriver,
          template <typename> class I = FaceDefaultDeriver > 
              class FaceSimp2: public FaceArityMax<BVT,BET,BFT,DumTT, A, B, C, D, E, F, G, H, I>  {};


}// end namespace
#endif
