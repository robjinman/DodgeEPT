#include "Common.hpp"
#include "ObjectContainer.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// ObjectContainer::ObjectContainer
//===========================================
ObjectContainer::ObjectContainer() {
   resize(1, 1);
}

//===========================================
// ObjectContainer::insert
//===========================================
void ObjectContainer::insert(shared_ptr<EptObject> obj) {
   Vec2i& seg = EptObjAccessor::segment(*obj);

   bool global = false;

   if (seg.x < 0 || seg.y < 0) {
      if (seg.x == -1 && seg.y == -1)
         global = true;
      else
         EXCEPTION("Index out of range");
   }

   if (!global) resize(seg.x, seg.y);

   m_objects.insert(obj);

   switch (obj->type()) {
      case EptObject::PROTOTYPE: m_prototypes.insert(obj); break;
      case EptObject::INSTANCE:  m_instances.insert(obj);  break;
      default: assert(false);
   }

   m_byId.insert(make_pair(obj->id(), obj));
   m_byName.insert(make_pair(obj->name(), obj));

   if (global)
      m_globals.insert(obj);
   else
      m_bySegment[seg.x][seg.y].insert(obj);
}

//===========================================
// ObjectContainer::contains
//===========================================
bool ObjectContainer::contains(const QString& name) const {
   return m_byName.find(name) != m_byName.end();
}

//===========================================
// ObjectContainer::get
//===========================================
const ObjectContainer::wkPtrSet_t& ObjectContainer::get(int i, int j) const {
   if (i == -1 && j == -1) return m_globals;
   if (i < 0) EXCEPTION("Index out of range");
   if (j < 0) EXCEPTION("Index out of range");
   if (i >= static_cast<int>(m_bySegment.size())) EXCEPTION("Index out of range");
   if (j >= static_cast<int>(m_bySegment[i].size())) EXCEPTION("Index out of range");

   return m_bySegment[i][j];
}

//===========================================
// ObjectContainer::move
//===========================================
void ObjectContainer::move(const QString& name, int i, int j) {
   auto it = m_byName.find(name);

   if (it == m_byName.end())
      EXCEPTION("Object '" << name.toLocal8Bit().data() << "' not found");

   auto ptr = it->second.lock();
   assert(ptr);

   Vec2i& seg = EptObjAccessor::segment(*ptr);

   int x = seg.x;
   int y = seg.y;
   seg = Dodge::Vec2i(i, j);

   m_globals.erase(ptr);
   insert(ptr);
   m_bySegment[x][y].erase(ptr);
}

//===========================================
// ObjectContainer::changeType
//===========================================
void ObjectContainer::changeType(const QString& name, EptObject::type_t type) {
   auto i = m_byName.find(name);
   if (i == m_byName.end())
      EXCEPTION("No object with name '" << name.toLocal8Bit().data() << "'");

   auto ptr = i->second.lock();
   assert(ptr);

   EptObjAccessor::type(*ptr) = type;

   if (type == EptObject::PROTOTYPE) {
      m_instances.erase(ptr);
   }
   else if (type == EptObject::INSTANCE) {
      m_prototypes.erase(ptr);
   }
   else
      assert(false);

   insert(ptr);
}

//===========================================
// ObjectContainer::resize
//
// Ignores requests to down-size
//===========================================
void ObjectContainer::resize(int i, int j) {
   if (i < 0) EXCEPTION("Index out of range");
   if (j < 0) EXCEPTION("Index out of range");

   if (i > static_cast<int>(m_bySegment.size()))
      m_bySegment.resize(i);

   assert(m_bySegment.size() > 0);

   if (j > static_cast<int>(m_bySegment[0].size())) {
      for (auto it = m_bySegment.begin(); it != m_bySegment.end(); ++it)
         it->resize(j);
   }
}
