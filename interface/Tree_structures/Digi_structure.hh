#ifndef DIGI_STUCTURE_H
#define DIGI_STUCTURE_H

#ifndef NOVAL_I
#define NOVAL_I -9999
#endif
#ifndef NOVAL_F
#define NOVAL_F -9999.0
#endif

class DigiData {
 public:
  int i; // serial num of digi in the given module
  int row;
  int col;
  int adc;
  
  std::string list;
  
  DigiData() { init(); }
  void init()
  {
    i=NOVAL_I;
    row=NOVAL_I;
    col=NOVAL_I;
    adc=NOVAL_I;
    list="i/I:row:col:adc";
  };

  void* get(std::string field_name_p)
  {
    if(field_name_p == "i") { return &(this -> i); }
    if(field_name_p == "row")  { return &(this -> row); }
    if(field_name_p == "col")  { return &(this -> col); }
    if(field_name_p == "adc")  { return &(this -> adc); }
    std::cerr << "Error: DigiData::get() is unable to find the field: " << field_name_p << ". " << std::endl;
    exit(-1);
  }
  
};

#endif