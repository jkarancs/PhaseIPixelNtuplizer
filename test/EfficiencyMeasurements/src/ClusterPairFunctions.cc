#include "../interface/ClusterPairFunctions.h"

namespace ClusterPairFunctions
{
	void printClusterFieldInfo_(const Cluster& clusterField)
	{
		std::cout << "--- Begin cluster field info ---" << std::endl;
		std::cout << "det: "          << clusterField.mod_on.det    << "." << std::endl;
		std::cout << "layer: "        << clusterField.mod_on.layer  << "." << std::endl;
		std::cout << "module: "       << clusterField.mod_on.module << "." << std::endl;
		std::cout << "ladder: "       << clusterField.mod_on.ladder << "." << std::endl;
		std::cout << "x: "            << clusterField.x                << "." << std::endl;
		std::cout << "y: "            << clusterField.y                << "." << std::endl;
		std::cout << "clustSizeX: "   << clusterField.sizeX        << "." << std::endl;
		std::cout << "clustSizeY: "   << clusterField.sizeY        << "." << std::endl;
		std::cout << "clustIndex: "   << clusterField.clusterIndex       << "." << std::endl;
		std::cout << "clustSize: "    << clusterField.clusterSize        << "." << std::endl;
		std::cout << "charge: "       << clusterField.charge        << "." << std::endl;
		std::cout << "pixelsCol:    vector of size: "  << clusterField.pixelsCol.size()    << ". First element: " << clusterField.pixelsCol[0]    << "." << std::endl;
		std::cout << "pixelsRow:    vector of size: "  << clusterField.pixelsRow.size()    << ". First element: " << clusterField.pixelsRow[0]    << "." << std::endl;
		std::cout << "pixelsAdc:    vector of size: "  << clusterField.pixelsAdc.size()    << ". First element: " << clusterField.pixelsAdc[0]    << "." << std::endl;
		std::cout << "pixelsMarker: vector of size: "  << clusterField.pixelsMarker.size() << ". First element: " << clusterField.pixelsMarker[0] << "." << std::endl;
		std::cout << "--- End cluster field info ---" << std::endl;
	}

	void calcPairEndsAndOrderLeftRight_(Cluster const*& lhs, Cluster const*& rhs, int& lhsColMin, int& lhsColMax, int& rhsColMin, int& rhsColMax)
	{
		std::tie(lhsColMin, lhsColMax) = deref_minmax_element(lhs -> pixelsCol.begin(), lhs -> pixelsCol.end());
		std::tie(rhsColMin, rhsColMax) = deref_minmax_element(rhs -> pixelsCol.begin(), rhs -> pixelsCol.end());
		if(rhsColMin < lhsColMin)
		{
			// std::cout << "Before swap: " << lhs << " " << rhs << std::endl;
			std::cout << "SWAP!!!" << std::endl;
			std::swap(lhs, rhs);
			std::swap(lhsColMin, rhsColMin);
			std::swap(lhsColMax, rhsColMax);
		}
	}

	int calcPairXWidth(const PairType& pair)
	{
		int min1, min2, max1, max2;
		std::tie(min1, max1) = deref_minmax_element(pair.first  -> pixelsRow.begin(), pair.first  -> pixelsRow.end());
		std::tie(min2, max2) = deref_minmax_element(pair.second -> pixelsRow.begin(), pair.second -> pixelsRow.end());
		// if(max1 > min2 && max2 > min1)
		if(!areClustersPair(*pair.first, *pair.second))
		{
			std::cout << "BAD PAIRS!!!" << std::endl;
			std::cout << "(" << min1 << ", " << max1 << "), (" << min2 << ", " << max2 << ")" << std::endl;
		}
		int xWidth = std::max(max1, max2) - std::min(min1, min2) + 1;
		return xWidth;
	}

	int isClusterTagged(const Cluster& cluster)
	{
		for(const auto& pixelTag: cluster.pixelsMarker)
		{
			if(pixelTag != 0) return 1;
		}
		return 0;
	}

	int isClusterTaggedInColumn(const Cluster& clusterField, int col)
	{
		for(int i: range(clusterField.pixelsCol.size()))
		{
			if(clusterField.pixelsCol[i] == col && clusterField.pixelsMarker[i] != 0) return 1;		
		}
		return 0;
	}

	int areClustersPair(const Cluster& first, const Cluster& second)
	{
		if(!(first.mod_on == second.mod_on)) return 0;
		if(5 < std::abs(first.x - second.x)) return 0;
		const Cluster* lhs = &first;
		const Cluster* rhs = &second;
		int lhsColMin, lhsColMax, rhsColMin, rhsColMax;
		calcPairEndsAndOrderLeftRight_(lhs, rhs, lhsColMin, lhsColMax, rhsColMin, rhsColMax);
		if(lhsColMax % 2 != 1 || lhsColMax + 3 != rhsColMin) return 0;
		if(lhsColMax > rhsColMin && rhsColMax > lhsColMin)
		{
			std::cout << "+++BAD PAIRS!!!" << std::endl;
			std::cout << "(" << lhsColMin << ", " << lhsColMax << "), (" << rhsColMin << ", " << rhsColMax << ")" << std::endl;
		}
		return 1;
	}

	int areClustersEndTaggedPair(const Cluster& first, const Cluster& second)
	{
		if(!(first.mod_on == second.mod_on)) return 0;
		if(5 < std::abs(first.x - second.x)) return 0;
		const Cluster* lhs = &first;
		const Cluster* rhs = &second;
		int lhsColMin, lhsColMax, rhsColMin, rhsColMax;
		calcPairEndsAndOrderLeftRight_(lhs, rhs, lhsColMin, lhsColMax, rhsColMin, rhsColMax);
		if(lhsColMax % 2 != 1 || lhsColMax + 3 != rhsColMin) return 0;
		if(!isClusterTaggedInColumn(*lhs, lhsColMax)) return 0;
		if(!isClusterTaggedInColumn(*rhs, rhsColMin)) return 0;
		return 1;
	}

	PairCollectionType getClusterPairCollection(const std::vector<Cluster>& clusterCollection)
	{
		PairCollectionType clusterPairCollection;
		for(auto firstClusterIt = clusterCollection.begin(); firstClusterIt != clusterCollection.end(); ++firstClusterIt)
		{
			const ModuleData& mod1 = firstClusterIt -> mod_on;
			if(mod1.det != 0) continue;
			for(auto secondClusterIt = firstClusterIt + 1; secondClusterIt != clusterCollection.end(); ++secondClusterIt)
			{
				const ModuleData& mod2 = secondClusterIt -> mod_on;
				if(!(mod1 == mod2)) continue;
				if(!areClustersPair(*firstClusterIt, *secondClusterIt)) continue;
				clusterPairCollection.emplace_back(std::make_shared<Cluster>(*firstClusterIt), std::make_shared<Cluster>(*secondClusterIt));
			}
		}
		return clusterPairCollection;
	}

	PairCollectionType getClusterPairCollection(const SharedPtrCollectionType& clusterCollection)
	{
		PairCollectionType clusterPairCollection;
		for(auto firstClusterIt = clusterCollection.begin(); firstClusterIt != clusterCollection.end(); ++firstClusterIt)
		{
			const ModuleData& mod1 = (*firstClusterIt) -> mod_on;
			if(mod1.det != 0) continue;
			for(auto secondClusterIt = firstClusterIt + 1; secondClusterIt != clusterCollection.end(); ++secondClusterIt)
			{
				const ModuleData& mod2 = (*secondClusterIt) -> mod_on;
				if(!(mod1 == mod2)) continue;
				if(!areClustersPair(**firstClusterIt, **secondClusterIt)) continue;
				clusterPairCollection.emplace_back(*firstClusterIt, *secondClusterIt);
			}
		}
		return clusterPairCollection;
	}

	// The complexity of the calculations can be decreased by looping once and looking for every weighted avarage to calculate
	float getAdcWeightedRowCoordinateInCol(const Cluster& cluster, const int& col)
	{
		int rowSum      = 0;
		int adcSum      = 0;
		for(int pixelIndex: range(cluster.pixelsCol.size())) {
			if(cluster.pixelsCol[pixelIndex] == col)
			{
				rowSum += cluster.pixelsRow[pixelIndex] * cluster.pixelsAdc[pixelIndex];
				adcSum += cluster.pixelsAdc[pixelIndex];
			}
		}
		return static_cast<float>(rowSum) / adcSum;
	}

	float getClusterIndAngle(const Cluster& cluster)
	{
		int colMin, colMax;
		std::tie(colMin, colMax) = deref_minmax_element(cluster.pixelsCol.begin(), cluster.pixelsCol.end());
		float startRow = getAdcWeightedRowCoordinateInCol(cluster, colMin);
		float endRow   = getAdcWeightedRowCoordinateInCol(cluster, colMax);
		return std::atan2(colMax - colMin, endRow - startRow);
	}

	// If this is slow, the speed can be increased to almost 2x by only looping once on the collection and only calculating
	// the min and max values required. Finding both the min and max together should not be that slow though.
	float getClusterPairAngle(const PairType& pair)
	{
		int lhsColMin, lhsColMax, rhsColMin, rhsColMax;
		std::tie(lhsColMin, lhsColMax) = deref_minmax_element(pair.first  -> pixelsCol.begin(), pair.first  -> pixelsCol.end());
		std::tie(rhsColMin, rhsColMax) = deref_minmax_element(pair.second -> pixelsCol.begin(), pair.second -> pixelsCol.end());
		int lhsFirst = (lhsColMin < rhsColMin);
		const int& startCol = lhsFirst ? lhsColMin : rhsColMin;
		const int& endCol   = lhsFirst ? rhsColMax : lhsColMax;
		float startRow = getAdcWeightedRowCoordinateInCol(*pair.first,  startCol);
		float endRow   = getAdcWeightedRowCoordinateInCol(*pair.second, endCol);
		return std::atan2(endCol - startCol, endRow - startRow); 
	}

	std::vector<float> getClusterPairAngles(const std::vector<Cluster>& clusterCollection)
	{
		std::vector<float> clusterPairAngles;
		PairCollectionType clusterPairCollection(getClusterPairCollection(clusterCollection));
		for(const PairType& clusterPair: clusterPairCollection)
		{
			clusterPairAngles.emplace_back(getClusterPairAngle(clusterPair));
		}
		return clusterPairAngles;
	}
	TH1D getClusterPairAngles(const std::vector<Cluster>& clusterCollection, const std::string& histoName, const std::string& histoTitle) 
	{
		std::vector<float> pairAngles = getClusterPairAngles(clusterCollection);
		TH1D pairAnglesPlot(histoName.c_str(), histoTitle.c_str(), 100, 0.0, 3.15);
		for(const float& angle: pairAngles)
		{
			pairAnglesPlot.Fill(angle);
		}
		return pairAnglesPlot;
	}

	SharedPtrCollectionType getClustersWithSize(const std::vector<Cluster>& clusterCollection, const int& length) 
	{
		return(fmap(
			filter(clusterCollection, [&length] (const Cluster& cluster) 
			{
				return cluster.clusterSize == length;
			}), 
			[] (Cluster& cluster) 
			{
				return std::make_shared<Cluster>(cluster);
			}));
	}

	SharedPtrCollectionType getClustersWithXLength(const std::vector<Cluster>& clusterCollection, const int& length) 
	{
		return(fmap(
			filter(clusterCollection, [&length] (const Cluster& cluster) 
			{
				return cluster.sizeX == length;
			}), 
			[] (Cluster& cluster) 
			{
				return std::make_shared<Cluster>(cluster);
			}));
	}

	SharedPtrCollectionType getClustersWithYLength(const std::vector<Cluster>& clusterCollection, const int& length) 
	{
		return(fmap(
			filter(clusterCollection, [&length] (const Cluster& cluster) 
			{
				return cluster.sizeY == length;
			}), 
			[] (Cluster& cluster) 
			{
				return std::make_shared<Cluster>(cluster);
			}));
	}

	// Healthy: not tagged, unhealthy: tagged
	SharedPtrCollectionType getHealthyClustersWithSize(const std::vector<Cluster>& clusterCollection, const int& length)
	{
		return filter(getClustersWithSize(clusterCollection, length), [] (const std::shared_ptr<Cluster>& clusterPtr) 
		{
			return !isClusterTagged(*clusterPtr);
		});
	}

	// Healthy: not tagged, unhealthy: tagged
	SharedPtrCollectionType getHealthyClustersWithXLength(const std::vector<Cluster>& clusterCollection, const int& length)
	{
		return filter(getClustersWithXLength(clusterCollection, length), [] (const std::shared_ptr<Cluster>& clusterPtr) 
		{
			return !isClusterTagged(*clusterPtr);
		});
	}
	
	// Healthy: not tagged, unhealthy: tagged
	SharedPtrCollectionType getHealthyClustersWithYLength(const std::vector<Cluster>& clusterCollection, const int& length)
	{
		return filter(getClustersWithYLength(clusterCollection, length), [] (const std::shared_ptr<Cluster>& clusterPtr) 
		{
			return !isClusterTagged(*clusterPtr);
		});
	}

	// Healthy: not tagged, unhealthy: tagged
	SharedPtrCollectionType getUnhealthyClustersWithSize(const std::vector<Cluster>& clusterCollection, const int& length)
	{
		return filter(getClustersWithSize(clusterCollection, length), [] (const std::shared_ptr<Cluster>& clusterPtr) 
		{
			return isClusterTagged(*clusterPtr);
		});
	}

	// Healthy: not tagged, unhealthy: tagged
	SharedPtrCollectionType getUnhealthyClustersWithXLength(const std::vector<Cluster>& clusterCollection, const int& length)
	{
		return filter(getClustersWithXLength(clusterCollection, length), [] (const std::shared_ptr<Cluster>& clusterPtr) 
		{
			return isClusterTagged(*clusterPtr);
		});
	}

	// Healthy: not tagged, unhealthy: tagged
	SharedPtrCollectionType getUnhealthyClustersWithYLength(const std::vector<Cluster>& clusterCollection, const int& length)
	{
		return filter(getClustersWithYLength(clusterCollection, length), [] (const std::shared_ptr<Cluster>& clusterPtr) 
		{
			return isClusterTagged(*clusterPtr);
		});
	}

	PairCollectionType getRealPairsWithXLength(const std::vector<Cluster>&  clusterCollection, const int& mergedLength)
	{
		const auto mergingLengthFilter  = [&mergedLength] (const PairType& pair) { return calcPairXWidth(pair) == mergedLength; };
		const auto isEndTaggedFilter   = [] (const PairType& pair) { return areClustersEndTaggedPair(*(pair.first), *(pair.second)); };
		return PairCollectionType(filter(filter(getClusterPairCollection(clusterCollection), mergingLengthFilter), isEndTaggedFilter));
	}

	PairCollectionType getRealPairsWithYLength(const std::vector<Cluster>&  clusterCollection, const int& mergedLength)
	{
		const auto mergingLengthFilter = [&mergedLength] (const PairType& pair) { return pair.first -> sizeY + pair.second -> sizeY + 2 == mergedLength; };
		const auto isEndTaggedFilter   = [] (const PairType& pair) { return areClustersEndTaggedPair(*(pair.first), *(pair.second)); };
		return PairCollectionType(filter(filter(getClusterPairCollection(clusterCollection), mergingLengthFilter), isEndTaggedFilter));
	}

	PairCollectionType getFakePairsWithXLength(const std::vector<Cluster>&  clusterCollection, const int& mergedLength)
	{
		const auto mergingLengthFilter  = [&mergedLength] (const PairType& pair) { return calcPairXWidth(pair) == mergedLength; };
		const auto isNotEndTaggedFilter = [] (const PairType& pair) { return !areClustersEndTaggedPair(*(pair.first), *(pair.second)); };
		return PairCollectionType(filter(filter(getClusterPairCollection(clusterCollection), mergingLengthFilter), isNotEndTaggedFilter));
	}

	PairCollectionType getFakePairsWithYLength(const std::vector<Cluster>&  clusterCollection, const int& mergedLength)
	{
		const auto mergingLengthFilter  = [&mergedLength] (const PairType& pair) { return pair.first -> sizeY + pair.second -> sizeY + 2 == mergedLength; };
		const auto isNotEndTaggedFilter = [] (const PairType& pair) { return !areClustersEndTaggedPair(*(pair.first), *(pair.second)); };
		return PairCollectionType(filter(filter(getClusterPairCollection(clusterCollection), mergingLengthFilter), isNotEndTaggedFilter));
	}

	// Merging length: Y length of the cluster pairs after merging. It is basically the lengths of the 
	// clusters added together plus two (for the dcol loss)
	PairCollectionType getClusterPairsWithMergingLength(const std::vector<Cluster>& clusterCollection, const int& mergedLength) 
	{
		const auto mergingLengthFilter = [&mergedLength] (const PairType& pair) { return pair.first -> sizeY + pair.second -> sizeY + 2 == mergedLength; };
		return PairCollectionType(filter(getClusterPairCollection(clusterCollection), mergingLengthFilter));
	}
} // ClusterPairFunctions
