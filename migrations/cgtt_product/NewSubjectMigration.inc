<?php
/**
 * Created by PhpStorm.
 * User: root
 * Date: 30/11/17
 * Time: 14:10
 */

class NewSubjectMigration extends CgttBaseMigration {
	public function __construct($arguments) {
		parent::__construct($arguments);
		$query = Database::getConnection($this->cgttConection, $this->cgttConectionKey)
			->select('tematica_noticia')
			->fields('tematica_noticia', array('idtematica_noticia','valor_es'
			));
		$query->orderBy('valor_es', 'ASC');




		$this->source = new MigrateSourceSQL($query, array(), NULL, array(map_joinable => FALSE));

		// Set up our destination - terms in the migrate_example_beer_styles
		// vocabulary (note that we pass the machine name of the vocabulary).
		$this->destination = new MigrateDestinationTerm('tem_tica');

		$this->map = new MigrateSQLMap($this->machineName,
			array(
				'idtematica_noticia' => array('type' => 'int',
					'length' => 255,
					'not null' => TRUE,
					'description' => 'Subject ID',
				)
			),
			MigrateDestinationTerm::getKeySchema()
		);

		$this->addFieldMapping('name', 'valor_es');

		$this->addUnmigratedDestinations(array(
			'description'
		));
	}

}
