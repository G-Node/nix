#include <pandora/DataArray.hpp>


namespace pandora {
DataArray::DataArray(File parentFile, Group thisGroup, std::string identifier) :
        		  EntityWithSources(&parentFile, thisGroup, identifier) {
	expansionOrigin(0.0);
	std::vector<double> coefficients;
	coefficients.push_back(1.0);
	polynomCoefficients(coefficients);
}

DataArray::DataArray(const DataArray &other) :
        		  EntityWithSources(other.file, other.group, other.id()) {
	expansionOrigin(0.0);
	std::vector<double> coefficients;
	coefficients.push_back(1.0);
	polynomCoefficients(coefficients);
}

std::string DataArray::label() const {
	std::string value;
	group.getAttr("label", value);
	return value;
}

void DataArray::label(const std::string &value) {
	group.setAttr("label", value);
}

std::string DataArray::unit() const {
	std::string value;
	group.getAttr("unit", value);
	return value;
}

void DataArray::unit(const std::string &value) {
	group.setAttr("unit", value);
}

void DataArray::expansionOrigin(double expansion_origin) {
	group.setAttr("expansion_origin", expansion_origin);
}

double DataArray::expansionOrigin() const {
	double expansion_origin;
	group.getAttr("expansion_origin", expansion_origin);
	return expansion_origin;
}
/*
void DataArray::polynomOrder(int order) {
  group.setAttr("polynom_order", order);
}
 */

int DataArray::polynomOrder() const {
	int order;
	group.getAttr("polynom_order", order);
	return order;
}

void DataArray::polynomCoefficients(std::vector<double> &coefficients){
	group.setAttr("polynom_order", coefficients.size());
	PSize start;
	DataSet ds((H5::DataSet()));
	if (group.hasData("polynom_coefficients")) {
		ds = group.openData("polynom_coefficients");
		start = {0};
	} else {
		Charon<std::vector<double> > charon(coefficients);
		PSize size = {1};
		PSize maxsize = {H5S_UNLIMITED};
		PSize chunks = DataSet::guessChunking(size, DataType::Double);
		ds = DataSet::create(group.h5Group(), charon.getFileType(),  "polynom_coefficients", size, &maxsize, &chunks);
		start = {0};
	}
	Selection fileSel = ds.createSelection();
	PSize count = {1};
	fileSel.select(count, start);
	ds.write(coefficients, fileSel);
}

std::vector<double> DataArray::polynomCoefficients()const{
	std::vector<double> c;
	if (group.hasData("polynom_coefficients")) {
		DataSet dataset = group.openData("polynom_coefficients");
		Selection fileSel = dataset.createSelection();
		PSize start = {0};
		PSize count = {1};
		fileSel.select(count, start);

		dataset.read(c, fileSel, true);
	}
	return c;
}

void DataArray::setCalibration(std::vector<double> &coefficients, double origin){

}

DataSet DataArray::data() {
	return group.openData("data");
}

} //namespace pandora
