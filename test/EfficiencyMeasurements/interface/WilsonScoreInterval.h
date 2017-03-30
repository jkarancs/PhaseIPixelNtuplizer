#pragma once

class WilsonScoreIntervalErrorCalculator
{
	private:
		double numMeasurements_;
		double measurementsResult_;
		double searchWindowInSigmas_;
	public:
		WilsonScoreIntervalErrorCalculator(const double& numMeasurements, const double& measurementsResult, const double& searchWindowInSigmas = 1.0): 
			numMeasurements_(numMeasurements),
			measurementsResult_(measurementsResult),
			searchWindowInSigmas_(searchWindowInSigmas) {};
		std::pair<double, double> getError()
		{
			if(numMeasurements_ == 0) return std::make_pair(0.0, 0.0);
			const double multiplier  = 1.0 / (1.0 + (1.0 / numMeasurements_) * searchWindowInSigmas_ * searchWindowInSigmas_);
			const double constant    = measurementsResult_ + (0.5 / numMeasurements_) * searchWindowInSigmas_ * searchWindowInSigmas_;
			const double differences = searchWindowInSigmas_ * std::sqrt((1.0 / numMeasurements_) * measurementsResult_ * (1.0 - measurementsResult_) + 0.25 / (numMeasurements_ * numMeasurements_) * searchWindowInSigmas_ * searchWindowInSigmas_);
			return std::make_pair(multiplier * (constant - differences), multiplier * (constant + differences));
		}
};
