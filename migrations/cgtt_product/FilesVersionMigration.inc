<?php
/**
 * Created by PhpStorm.
 * User: root
 * Date: 21/12/17
 * Time: 13:58
 */

class FilesVersionMigration extends CgttBaseMigration {

	public function __construct(array $arguments = array()) {
		parent::__construct($arguments);

		$this->description = t('Import files.');

		$query = Database::getConnection($this->cgttConection, $this->cgttConectionKey)
			->select('public_downloads')
			->fields('public_downloads', array('file_id',
				'release_id',
				'filename',
				'release_time',
				'post_date',
				'package_id',
				'group_id',
				'status_id',
			));

		$this->source = new MigrateSourceSQL($query, array(), NULL, array(map_joinable => FALSE));
		$this->destination = new MigrateDestinationFile('file', 'MigrateFileUri');


		// Save to the default file scheme.
		$this->addFieldMapping('destination_dir')
			->defaultValue(variable_get('file_default_scheme', 'public') . '://');
		// Use the full file path in the file name so that we retain the directory
		// structure.
		$this->addFieldMapping('destination_file', 'testing');
		// Set the value to the file name, including path.
		$this->addFieldMapping('value', 'filename');
		// Uncomment this if you want to replace existing files.
		// $this->addFieldMapping('file_replace')
		//   ->defaultValue(FILE_EXISTS_REPLACE);

	}


	public function prepareRow($currentRow) {
		$this->nextPreviousRelease = $this->previousRelease;
		$this->previousRelease = $currentRow->release_id;
		$this->previousFileName = $this->nextFileName;
		$this->nextFileName = $currentRow->filename;
		$currentRow->testing = $currentRow->filename;

		//Compare the releases
		if ($this->previousRelease == $this->nextPreviousRelease) {

			if ($this->counter == 0) {
				$this->zipFile = new ZipArchive;
			}

			if ($this->counter == 0 && $this->zipFile->open($this->filesPath.'Archivo-'.$currentRow->release_id.'.zip', ZipArchive::CREATE) === TRUE) {
				if (file_exists($this->filesPath . $this->previousFileName)) {
					$this->zipFile->addFile($this->filesPath . $this->previousFileName, $this->previousFileName);
					$this->zipFile->addFile($this->filesPath . $currentRow->filename, $currentRow->filename);
				}
			}
			elseif ($this->counter > 0) {
				if (file_exists($this->filesPath . $currentRow->filename)) {
					$this->zipFile->addFile($this->filesPath . $currentRow->filename, $currentRow->filename);
				}
			}
			$this->counter = $this->counter + 1;
			$this->versionName = $currentRow->name;


			return FALSE;
		}
		else {
			if ($this->counter > 0) {
				$currentRow->filename = $this->zipFile->filename;
				$currentRow->release_id = $currentRow->release_id - 1;
				$currentRow->name = $this->versionName;
				$this->zipFile->close();
				$this->counter = 0;
			}
		}

		drush_print_r($currentRow);

	}




}