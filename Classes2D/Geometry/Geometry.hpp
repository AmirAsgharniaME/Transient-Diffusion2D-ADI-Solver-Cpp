#pragma once


class Geometry
{
private:
    // Bar Cross section with Height and Width
    double Height;
    double Width;

public:
      explicit Geometry (double DefaultValue_ = 1.0); // explicit : Geometry G = 0.05; Not allowed

      //const :  Read-only: guarantees not to modify class members or call non-const functions.
      double GetHeight() const;
      double GetWidth() const;

      void SetHeight(double Value_);
      void SetWidth(double Value_);



};


