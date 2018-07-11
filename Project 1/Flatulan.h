//
//  Flatulan.hpp
//  CS32.Project.1
//
//  Created by Neta Tartakovsky on 1/10/18.
//  Copyright © 2018 Neta Tartakovsky. All rights reserved.
//

#ifndef Flatulan_Included
#define Flatulan_Included

class City;

class Flatulan
{
public:
    // Constructor
    Flatulan(City* cp, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    
    // Mutators
    void move();
    bool possiblyGetConverted();
    
private:
    City* m_city;
    int   m_row;
    int   m_col;
};

#endif // Flatulan_Included


