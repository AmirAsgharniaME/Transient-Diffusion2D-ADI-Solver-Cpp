#include "Classes2D/FileWriter2D/FileWriter2D.hpp"

void FileWriter2D::WriteField2D(
    Field2D& Field2D_Obj,  
    Mesh2D& Mesh2D_Obj ,
    FieldType FieldType_,
    std::string RelativePath)
{
    std::string FileName;
    if (FieldType_ == FieldType::Initial_Numerical)
    {
        std::cout<<"Initialization Completed, Writing The Initial Field2D To File..."<<std::endl;
        FileName = "Initial_Numerical2D";
    }
    else if (FieldType_ == FieldType::Steady_State_Numerical)
    {
        std::cout<<"Writing The Steady State Numerical Field2D To File..."<<std::endl;
        FileName = "Steady_State_Numerical2D";
    }

    std::string FileExtension = "dat";
    std::filesystem::path dirPath(RelativePath);
    if (!std::filesystem::exists(dirPath))
    {
        std::filesystem::create_directories(dirPath);
    }

    std::filesystem::path fullFilePath = dirPath / (FileName + "." + FileExtension);

    std::ofstream file(fullFilePath);


    if (file.is_open()) 
    {
    const std::size_t nrows = Field2D_Obj.GetNrows();
    const std::size_t ncols = Field2D_Obj.GetNcols();
        file << std::fixed << std::setprecision(6);
    for (std::size_t row = 0; row < nrows; ++row)
        {    
        //const std::size_t yIndex = nrows - 1 - row;
        for (std::size_t col = 0; col < ncols; ++col)
            {    
                file << std::left
                 << std::setw(20) << Mesh2D_Obj.GetXPositions(col)
                 << std::setw(20) << Mesh2D_Obj.GetYPositions(row)
                 << std::setw(20) << Field2D_Obj.GetValue(row, col)
                 << '\n';
            }
        }
                             
    std::cout << FileName << "." << FileExtension << " Created Successfully At: " << fullFilePath.string() << std::endl;
    file.close();
    }

    else 
    {
        std::cout << FileName << "." << FileExtension << " Failed To Create Because The Path Folder Is WRONG." << std::endl;
    } 

}

void FileWriter2D::WriteField2D(
    AnalyticalDiffusion2D& Analytical2D_Obj,  
    Mesh2D& Mesh2D_Obj ,
    [[maybe_unused]] FieldType FieldType_,
    std::string RelativePath)
{
    std::cout<<"Analytical Tempreture values have been calculated, Writing The Analytical velocity File"<<std::endl;
    std::string FileName = "Steady_State_Analytical2D";
    std::string FileExtension = "dat";
    std::filesystem::path dirPath(RelativePath);
    if (!std::filesystem::exists(dirPath))
    {
        std::filesystem::create_directories(dirPath);
    }

    std::filesystem::path fullFilePath = dirPath / (FileName + "." + FileExtension);

    std::ofstream file(fullFilePath);

    if (file.is_open()) 
    {
    const std::size_t nrows = Analytical2D_Obj.GetNrows();
    const std::size_t ncols = Analytical2D_Obj.GetNcols();
        file << std::fixed << std::setprecision(6);
    for (std::size_t row = 0; row < nrows; ++row)
        {    
        //const std::size_t yIndex = nrows - 1 - row;
        for (std::size_t col = 0; col < ncols; ++col)
            {    
                file << std::left
                 << std::setw(20) << Mesh2D_Obj.GetXPositions(col)
                 << std::setw(20) << Mesh2D_Obj.GetYPositions(row)
                 << std::setw(20) << Analytical2D_Obj.GetValue(row, col)
                 << '\n';
            }
        }
                             
    std::cout << FileName << "." << FileExtension << " Created Successfully At: " << fullFilePath.string() << std::endl;
    file.close();
    }

    else 
    {
        std::cout << FileName << "." << FileExtension << " Failed To Create Because The Path Folder Is WRONG." << std::endl;
    } 

}



