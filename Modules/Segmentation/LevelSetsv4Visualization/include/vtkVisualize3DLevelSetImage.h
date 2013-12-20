/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef __vtkVisualize3DLevelSetImage_h
#define __vtkVisualize3DLevelSetImage_h

#include "itkImage.h"

#include "itkLevelSetImage.h"
#include "itkImageToVTKImageFilter.h"
#include "itkLevelSetTovtkImageData.h"

#include "vtkVersion.h"

#include "vtkImageData.h"
#include "vtkMarchingCubes.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkImageMapper3D.h"
#include "vtkImageActor.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"
#include "vtkImageViewer2.h"
#include "vtkImageShiftScale.h"

#include "vtkCaptureScreen.h"
#include "vtkPNGWriter.h"

/**
 * \class vtkVisualize3DLevelSetImage
 * \tparam TInputImage Input Image Type
 * \tparam TLevelSetImage Level Set type
 *
 * \ingroup ITKLevelSetsv4Visualization
 */
template< class TInputImage, class TLevelSetImage >
class vtkVisualize3DLevelSetImage : public itk::LightObject
{
public:
  typedef vtkVisualize3DLevelSetImage     Self;
  typedef LightObject                     Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(vtkVisualize3DLevelSetImage, LightObject);

  typedef TInputImage     InputImageType;
  typedef TLevelSetImage  LevelSetImageType;

  typedef itk::LevelSetDenseImage< LevelSetImageType > LevelSetType;

  typedef itk::ImageToVTKImageFilter< InputImageType >  ImageConverterType;
  typedef typename ImageConverterType::Pointer          ImageConverterPointer;

  typedef itk::LevelSetTovtkImageData< LevelSetType > LevelSetConverterType;
  typedef typename LevelSetConverterType::Pointer     LevelSetConverterPointer;


  void SetInputImage( const InputImageType * iImage )
    {
    m_ImageConverter->SetInput( iImage );
    try
      {
      m_ImageConverter->Update();
      }
    catch( itk::ExceptionObject& e )
      {
      std::cout << e << std::endl;
      return;
      }

    m_Count = 0;
    }

  void SetLevelSet( const LevelSetType *f )
    {
    m_LevelSetConverter->SetInput( f );
    m_Count = 0;
    }

  void SetScreenCapture( const bool& iCapture )
    {
    m_ScreenCapture = iCapture;
    }

  void Update()
    {
    try
      {
      m_LevelSetConverter->Update();
      }
    catch( itk::ExceptionObject& e )
      {
      std::cout << e << std::endl;
      return;
      }

    const int LevelSet_id = 0;
    vtkSmartPointer< vtkMarchingCubes > contours =
      vtkSmartPointer< vtkMarchingCubes >::New();
#if VTK_MAJOR_VERSION <= 5
    contours->SetInput( m_ImageConverter->GetOutput() );
#else
    m_ImageConverter->Update();
    contours->SetInputData( m_ImageConverter->GetOutput() );
#endif
    contours->GenerateValues( LevelSet_id, 0, 0 );

    vtkSmartPointer< vtkPolyDataMapper > mapper =
        vtkSmartPointer< vtkPolyDataMapper >::New();
    mapper->SetInputConnection( contours->GetOutputPort() );
    mapper->SetScalarRange( -10, 10 );

    vtkSmartPointer< vtkActor > ContourActor =
        vtkSmartPointer< vtkActor >::New();
    ContourActor->SetMapper( mapper );
    ContourActor->GetProperty()->SetLineWidth( 2. );
    ContourActor->GetProperty()->SetColor( 0, 0, 1 );
    ContourActor->GetProperty()->SetOpacity( 1.0 );

    vtkSmartPointer< vtkImageShiftScale > shift =
        vtkSmartPointer< vtkImageShiftScale >::New();
#if VTK_MAJOR_VERSION <= 5
    shift->SetInput( m_ImageConverter->GetOutput() );
#else
    m_ImageConverter->Update();
    shift->SetInputData( m_ImageConverter->GetOutput() );
#endif
    shift->SetOutputScalarTypeToUnsignedChar();
    shift->Update();

    vtkSmartPointer< vtkImageActor > input_Actor =
        vtkSmartPointer< vtkImageActor >::New();
#if VTK_MAJOR_VERSION <= 5
    input_Actor->SetInput( shift->GetOutput() );
#else
    input_Actor->GetMapper()->SetInputConnection( shift->GetOutputPort() );
#endif

    vtkSmartPointer< vtkRenderer > ren =
        vtkSmartPointer< vtkRenderer >::New();
    ren->SetBackground( 0.5, 0.5, 0.5 );

    vtkSmartPointer< vtkRenderWindowInteractor > iren =
        vtkSmartPointer< vtkRenderWindowInteractor >::New();

    vtkSmartPointer< vtkRenderWindow > renWin =
        vtkSmartPointer< vtkRenderWindow >::New();

    ren->AddActor ( input_Actor );
    ren->AddActor ( ContourActor );

    renWin->AddRenderer( ren );

    if( m_ScreenCapture )
      {
      iren->SetRenderWindow( renWin );

      renWin->Render();

      std::string filename;
      std::stringstream yo;
      yo << "snapshot_" << m_Count;
      filename = yo.str();
      filename.append ( ".png" );

      vtkCaptureScreen< vtkPNGWriter > capture ( renWin );
      // begin mouse interaction
      iren->Start();
      capture( filename );
      ++m_Count;
      }
    else
      {
      renWin->Render();
      }
    }

protected:
  vtkVisualize3DLevelSetImage() : Superclass(),
    m_Count( 0 ),
    m_ScreenCapture( false )
    {
    m_ImageConverter = ImageConverterType::New();
    m_LevelSetConverter = LevelSetConverterType::New();
    }

  ~vtkVisualize3DLevelSetImage()
    {}

private:
  vtkVisualize3DLevelSetImage ( const Self& );
  void operator = ( const Self& );

  ImageConverterPointer     m_ImageConverter;
  LevelSetConverterPointer  m_LevelSetConverter;

  itk::IdentifierType m_Count;
  bool                m_ScreenCapture;

  };
#endif
