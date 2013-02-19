/***
 * Demonstrike Core
 */

#pragma once

#include "../g3dlite/Matrix3.h"
#include "../g3dlite/Vector3.h"
#include "../g3dlite/AABox.h"
#include "../g3dlite/Ray.h"
#include "../../Common.h"

namespace VMAP
{
	class WorldModel;
	struct AreaInfo;
	struct LocationInfo;

	enum ModelFlags
	{
		MOD_M2 = 1,
		MOD_WORLDSPAWN = 1<<1,
		MOD_HAS_BOUND = 1<<2
	};

	class ModelSpawn
	{
		public:
			//mapID, tileX, tileY, Flags, ID, Pos, Rot, Scale, Bound_lo, Bound_hi, name
			uint32 flags;
			uint16 adtId;
			uint32 ID;
			G3D::Vector3 iPos;
			G3D::Vector3 iRot;
			float iScale;
			G3D::AABox iBound;
			std::string name;
			bool operator==(const ModelSpawn &other) const { return ID == other.ID; }
			//uint32 hashCode() const { return ID; }
			// temp?
			const G3D::AABox& getBounds() const { return iBound; }


			static bool readFromFile(FILE *rf, ModelSpawn &spawn);
			static bool writeToFile(FILE *rw, const ModelSpawn &spawn);
	};

	class ModelInstance: public ModelSpawn
	{
		public:
			ModelInstance(): iModel(0) {}
			ModelInstance(const ModelSpawn &spawn, WorldModel *model);
			void setUnloaded() { iModel = 0; }
			bool intersectRay(const G3D::Ray& pRay, float& pMaxDist, bool pStopAtFirstHit) const;
			void intersectPoint(const G3D::Vector3& p, AreaInfo &info) const;
			bool GetLocationInfo(const G3D::Vector3& p, LocationInfo &info) const;
			bool GetLiquidLevel(const G3D::Vector3& p, LocationInfo &info, float &liqHeight) const;
		protected:
			G3D::Matrix3 iInvRot;
			float iInvScale;
			WorldModel *iModel;

#ifdef MMAP_GENERATOR
		public:
			WorldModel* const getWorldModel();
#endif
	};
} // namespace VMAP
