/**
 * @file
 * @addtogroup WorldEd World editor
 *
 * Copyright Broken Singularity, All Rights Reserved.
 * Authors: Yehor Pohuliaka (zombiHello)
 */

#ifndef TEXTUREEDITORWINDOW_H
#define TEXTUREEDITORWINDOW_H

#include <QWidget>
#include "Render/Texture.h"

namespace Ui
{
	class WeTextureEditorWindow;
}

/**
 * @ingroup WorldEd
 * Class window of texture editor
 */
class WeTextureEditorWindow : public QWidget
{
	Q_OBJECT

public:
	/**
	 * Constructor
	 * 
	 * @param InTexture2D	Texture 2D to edit
	 * @param InParent		Parent widget
	 */
	WeTextureEditorWindow( const TSharedPtr<FTexture2D>& InTexture2D, QWidget* InParent = nullptr );

	/**
	 * Destructor
	 */
	virtual ~WeTextureEditorWindow();

private slots:
	//
	// Events
	//
	void on_comboBox_addressU_currentIndexChanged( int InIndex );
	void on_comboBox_addressV_currentIndexChanged( int InIndex );
	void on_comboBox_filter_currentIndexChanged( int InIndex );
	void on_toolButton_sourceFile_clicked();
	void on_toolButton_sourceFileRemove_clicked();
	void on_actionReimport_triggered();
	void on_actionR_toggled( bool InValue );
	void on_actionG_toggled( bool InValue );
	void on_actionB_toggled( bool InValue );
	void on_actionA_toggled( bool InValue );

signals:
	/**
	 * Event when texture is changed
	 * 
	 * @param InAsset		Changed asset
	 */
	void OnChangedAsset( const TSharedPtr<FAsset>& InAsset );

private:
	/**
	 * Init UI
	 */
	void InitUI();

	/**
	 * Called when source file changed
	 * 
	 * @param InNewSourceFile		New path to source file
	 */
	void OnSourceFileChanged( QString InNewSourceFile );

	/**
	 * Crop source file text in UI
	 */
	void UICropSourceFileText();

	/**
	 * Event called when resize widget
	 *
	 * @param[in] InEvent Event of resize widget
	 */
	virtual void resizeEvent( QResizeEvent* InEvent ) override;

	bool									bInit;								/**< Is inited window */
	Ui::WeTextureEditorWindow*				ui;									/**< Qt UI */
	TSharedPtr<FTexture2D>					texture2D;							/**< Editable asset */
	class FTexturePreviewViewportClient*	viewportClient;						/**< Viewport client */
	class QComboBox*						comboBox_addressU;					/**< Combobox for select address mode by U */
	class QComboBox*						comboBox_addressV;					/**< Combobox for select address mode by V */
	class QComboBox*						comboBox_filter;					/**< Combobox for select sampler filter */
	class QLabel*							label_sourceFileValue;				/**< Label for show path to source file */
	class QToolButton*						toolButton_sourceFile;				/**< Tool button for select new path to source file */
	class QToolButton*						toolButton_sourceFileRemove;		/**< Tool button for remove path to source file */
};

#endif // MAINWINDOW_H