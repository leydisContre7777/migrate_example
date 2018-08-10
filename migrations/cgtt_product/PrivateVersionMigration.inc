<?php

/**
 * Created by Leydis Contreras.
 * User: root
 * Date: 18/12/17
 * Time: 10:28
 */
class PrivateVersionMigration extends CgttBaseMigration {
	protected  $previousRelease;
	protected  $nextPreviousRelease;
	protected  $zipFile;
	protected  $counter = 0;
	protected  $fileName;
	protected  $nextFileName;
	protected  $previousFileName;
	protected  $filesPath;
	protected  $versionName;
	protected  $file;
	protected  $setter = 0;


	/**
	 * Initialization of Public Migration object.
	 */
	public function __construct($arguments) {
		parent::__construct($arguments);
		$query = Database::getConnection($this->cgttConection, $this->cgttConectionKey)
			->select('private_downloads')
			->fields('private_downloads', array('file_id',
				'name',
				'release_id',
				'filename',
				'group_id',
				'status_id',
				'testing',
				'date'
			));
		$query->join('groups', 'g', 'g.group_id = private_downloads.group_id');
		$query->fields('g', array('group_name'));

		$this->filesPath = variable_get('file_public_path', conf_path() . '/files').'/';


		$this->source = new MigrateSourceSQL($query, array(), NULL, array(map_joinable => FALSE));
		$this->destination = new MigrateDestinationNode('version_producto_privada');

		$this->map = new MigrateSQLMap($this->machineName,
			array(
				'file_id' => array('type' => 'int',
					'unsigned' => TRUE,
					'not null' => TRUE,
					'description' => t('Source ID'),
				)
			),
			MigrateDestinationNode::getKeySchema()
		);


		$this->addFieldMapping('title', 'name');
		$this->addFieldMapping('field_estado_version')->defaultValue('Finalizada');

		$this->addFieldMapping('body', 'release_id');

		$this->addFieldMapping('og_group_ref', 'og_group_ref');

		$this->addFieldMapping('field_c_digo_versi_n_producto', 'testing');


		$this->addFieldMapping('field_c_digo_versi_n_producto:source_dir')->defaultValue($this->filesPath.'/docs');
		$this->addFieldMapping('field_c_digo_versi_n_producto:destination_file', 'testing');


		//ignored
		$this->addFieldMapping('field_c_digo_versi_n_producto:file_class')->issueGroup(t('DNM'));
		$this->addFieldMapping('field_c_digo_versi_n_producto:preserve_files')->issueGroup(t('DNM'));
		$this->addFieldMapping('field_c_digo_versi_n_producto:destination_dir')->issueGroup(t('DNM'));
		$this->addFieldMapping('field_c_digo_versi_n_producto:file_replace')->issueGroup(t('DNM'));
		$this->addFieldMapping('field_c_digo_versi_n_producto:urlencode')->issueGroup(t('DNM'));
		$this->addFieldMapping('field_c_digo_versi_n_producto:description')->issueGroup(t('DNM'));
		$this->addFieldMapping('field_c_digo_versi_n_producto:display')->issueGroup(t('DNM'));
		$this->addFieldMapping('status')->defaultValue(1);
		$this->addFieldMapping('changed', 'date');
		$this->addFieldMapping('created', 'date');

		$this->addFieldMapping('language')->defaultValue('es');



		$this->addUnmigratedDestinations(array(
			'body:summary',
			'field_notas_de_version',
			'field_notas_de_version:destination_dir',
			'field_notas_de_version:source_dir',
			'field_notas_de_version:file_class',
			'field_notas_de_version:file_replace',
			'field_notas_de_version:urlencode',
			'field_notas_de_version:description',
			'field_notas_de_version:preserve_files',
			'field_notas_de_version:destination_file',
			'field_notas_de_version:display',
			'field_estado_version:source_typ',
			'field_estado_version:create_term',
			'field_estado_version:ignore_case',
			'field_estado_version:source_type',
			'field_usuarios_autorizados',
			'body:format',
			'comment',
			'is_new',
			'log',
			'promote',
			'revision',
			'revision_uid',
			'tnid',
			'totalcount',
			'daycount',
			'timestamp',
			'path',
			'translate',
			'sticky',
			'uid'));

	}


	/**
	 * Default implementation of prepareRow(). This method is called from the source
	 * plugin upon first pulling the raw data from the source.
	 * @todo create zip file when the realease_id is repeated.
	 * @param $current_row
	 *  Object containing raw source data.
	 * @return bool
	 *  TRUE to process this row, FALSE to have the source skip it.
	 */
	public function prepareRow($currentRow) {
		$result = db_select('node', 'n')
			->fields('n', array('nid', 'title'))
			->condition('title', '%'.db_like($currentRow->group_name).'%', 'LIKE')
			->condition('n.type', 'producto')
			->execute()
			->fetchAll();


		$currentRow->og_group_ref = $result[0]->nid;

		$this->nextPreviousRelease = $this->previousRelease;
		$this->previousRelease = $currentRow->release_id;
		$this->previousFileName = $this->nextFileName;
		$this->nextFileName = $currentRow->filename;
		$currentRow->testing = $currentRow->filename;

		//Compare the releases
		if ($this->previousRelease == $this->nextPreviousRelease) {
			//Delete the node because Drupal is saving the node without the zip file
			$nodes = node_load_multiple(NULL, array('title' =>$currentRow->name));
			$node = current($nodes);

			if ($node->nid) {
				node_delete($node->nid);
			}
			//Initialize the zip
			if ($this->counter == 0 && file_exists($this->filesPath . $currentRow->filename)) {
				$this->zipFile = new ZipArchive;
				$this->setter = 1;

			} else {
				$this->setter = 0;

			}

			//Open the zip file
			if (file_exists($this->filesPath . $currentRow->filename) && $this->counter == 0
				&& $this->zipFile->open($this->filesPath.'docs/Version-'.$currentRow->release_id.'.zip', ZipArchive::CREATE) === TRUE) {
				if (file_exists($this->filesPath . $this->previousFileName)) {

					//Adding file to the zip
					$this->zipFile->addFile($this->filesPath . $this->previousFileName, $this->previousFileName);
					$this->zipFile->addFile($this->filesPath . $currentRow->filename, $currentRow->filename);
					$this->file = 'Version-'.$currentRow->release_id.'.zip';
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
				if ($this->zipFile && $this->setter == 1) {
					$this->zipFile->close();
				}
				$this->counter = 0;
				$currentRow->testing = $this->file;
			}
		}
	}


	public function complete($entity, stdClass $row) {
		$transition = new WorkflowTransition();
		$transition->setValues('node', $entity, '', $entity->workflow, 39, 1, REQUEST_TIME, t('Updating workflow state'));
		$transition->execute(TRUE);
	}
}
