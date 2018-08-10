<?php

class ProjectMigration extends CgttBaseMigration {


	public function __construct($arguments) {
		parent::__construct($arguments);
		$query = Database::getConnection($this->cgttConection, $this->cgttConectionKey)
			->select('productos_new')
			->fields('productos_new', array('group_id',
				'group_name',
				'unix_group_name',
				'short_description',
				'register_purpose',
				'tematica',
				'licencia_soft',
				'tags',
				'estado_producto',
				'requerimientos',
				'empresas_colaboradoras',
				'website',
				'usersvn',
				'passwordsvn',
				'urlsvn',
				'urlsonar',
				'consejeria',
				'fecha'
			));
		$this->source = new MigrateSourceSQL($query, array(), NULL, array(map_joinable => FALSE));
		$this->destination = new MigrateDestinationNode('producto');

		$this->map = new MigrateSQLMap($this->machineName,
			array(
				'group_id' => array('type' => 'int',
					'unsigned' => TRUE,
					'not null' => TRUE,
					'description' => t('Source ID'),
				)
			),
			MigrateDestinationNode::getKeySchema()
		);

		$this->addFieldMapping('title', 'group_name');
		$this->addFieldMapping('field_nombre_corto', 'unix_group_name');
		$this->addFieldMapping('field_descripci_n_breve', 'unix_group_name');
		$this->addFieldMapping('body', 'short_description');
		$this->addFieldMapping('field_objetivos', 'register_purpose');
		$this->addFieldMapping('field_tem_tica_producto', 'tematica')->separator(',');
		$this->addFieldMapping('field_licencia_producto', 'licencia_soft');
		$this->addFieldMapping('field_tags_producto', 'tags')->separator(',');
		$this->addFieldMapping('group_group')
			->defaultValue(1);
		$this->addFieldMapping('field_estado_producto', 'estado_producto');
		$this->addFieldMapping('field_requerimientos_t_cnicos', 'requerimientos')->separator(',');
		$this->addFieldMapping('field_empresas_colaboradoras', 'empresas_colaboradoras')->separator(',');
		$this->addFieldMapping('field_sitio_web', 'website');
		$this->addFieldMapping('field_sistema_de_control_de_vers', 'svn');
		$this->addFieldMapping('field_usuario_svn', 'usersvn');
		$this->addFieldMapping('field_contrase_a_svn', 'passwordsvn');
		$this->addFieldMapping('field_url_svn', 'urlsvn');
		$this->addFieldMapping('field_url_sonar', 'urlsonar');
		$this->addFieldMapping('field_consejeria_producto', 'consejeria');
		$this->addFieldMapping('field_descargas')->defaultValue(1);
		$this->addFieldMapping('field_descargas_privadas')->defaultValue(1);
		$this->addFieldMapping('field_pestana_documentaci_n')->defaultValue(1);
		$this->addFieldMapping('field_comunidad')->defaultValue(1);
		$this->addFieldMapping('field_evoluci_n')->defaultValue(1);
		$this->addFieldMapping('field_noticias')->defaultValue(1);
		$this->addFieldMapping('field_foro')->defaultValue(1);
		$this->addFieldMapping('field_wiki')->defaultValue(1);
		$this->addFieldMapping('field_miembros')->defaultValue(1);
		$this->addFieldMapping('field_scv')->defaultValue(1);
		$this->addFieldMapping('field_tareas')->defaultValue(1);
		$this->addFieldMapping('status')->defaultValue(1);
		$this->addFieldMapping('language')->defaultValue('es');
		$this->addFieldMapping('changed', 'fecha');
		$this->addFieldMapping('created', 'fecha');




		$this->addUnmigratedDestinations(array(
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
			'uid',
			'field_categor_a_producto'
		));

	}

	public function prepareRow($row) {

		if ($row->requerimientos) {
			$row->requirements = explode(",", $row->requerimientos);

			$requirements_voc = array();
			foreach ($row->requirements as $value) {
				$term_array = taxonomy_get_term_by_name($value);
				$term = reset($term_array);
				array_push($requirements_voc, $term->tid);
			}

			$row->requirements = $requirements_voc;


		}

		return parent::prepareRow($row);
	}


	public function complete($entity, stdClass $row) {
		$transition = new WorkflowTransition();
		$transition->setValues('node', $entity, '', $entity->workflow, 39, 1, REQUEST_TIME, t('Updating workflow state'));
		$transition->execute(TRUE);
	}


}