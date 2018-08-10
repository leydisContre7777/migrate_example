<?php
/**
 * Created by PhpStorm.
 * User: root
 * Date: 31/01/18
 * Time: 9:54
 */

class DocumentationMigration extends  CgttBaseMigration {
	public function __construct($arguments) {
		parent::__construct($arguments);
		$query = Database::getConnection($this->cgttConection, $this->cgttConectionKey)
			->select('documentacion')
			->fields('documentacion', array('docid',
				'nombre',
				'descripcion',
				'documento',
				'tipologia',
				'producto',
				'fecha'
			));
		$this->source = new MigrateSourceSQL($query, array(), NULL, array(map_joinable => FALSE));
		$this->destination = new MigrateDestinationNode('documento_producto');

		$this->filesPath = variable_get('file_public_path', conf_path() . '/files') . '/';


		$this->map = new MigrateSQLMap($this->machineName,
			array(
				'docid' => array('type' => 'int',
					'unsigned' => TRUE,
					'not null' => TRUE,
					'description' => t('Source ID'),
				)
			),
			MigrateDestinationNode::getKeySchema()
		);

		$this->addFieldMapping('title', 'nombre');
		$this->addFieldMapping('body', 'descripcion');

		$this->addFieldMapping('og_group_ref', 'og_group_ref');

		$this->addFieldMapping('field_tipologia', 'tipologia');

		$this->addFieldMapping('field_documento:source_dir')->defaultValue($this->filesPath . '/docs');
		$this->addFieldMapping('field_documento:destination_file', 'documento');

		//ignore
		$this->addFieldMapping('field_documento:file_class')->issueGroup(t('DNM'));
		$this->addFieldMapping('field_documento:preserve_files')->issueGroup(t('DNM'));
		$this->addFieldMapping('field_documento:destination_dir')->issueGroup(t('DNM'));
		$this->addFieldMapping('field_documento:file_replace')->issueGroup(t('DNM'));
		$this->addFieldMapping('field_documento:urlencode')->issueGroup(t('DNM'));
		$this->addFieldMapping('field_documento:description')->issueGroup(t('DNM'));
		$this->addFieldMapping('field_documento:display')->issueGroup(t('DNM'));
		$this->addFieldMapping('status')->defaultValue(1);
		$this->addFieldMapping('created', 'fecha');
		$this->addFieldMapping('changed', 'fecha');
		$this->addFieldMapping('language')->defaultValue('es');




	}

	public function prepareRow($currentRow) {
		$result = db_select('node', 'n')
			->fields('n', array('nid', 'title'))
			->condition('title', '%'.db_like($currentRow->producto).'%', 'LIKE')
			->condition('n.type', 'producto')
			->execute()
			->fetchAll();

		$currentRow->og_group_ref = $result[0]->nid;


	}


	public function complete($entity, stdClass $row) {
		$transition = new WorkflowTransition();
		$transition->setValues('node', $entity, '', $entity->workflow, 39, 1, REQUEST_TIME, t('Updating workflow state'));
		$transition->execute(TRUE);
	}

}
