<?php
/**
 * Created by PhpStorm.
 * User: root
 * Date: 1/12/17
 * Time: 12:50
 */

class TagTermMigration extends CgttBaseMigration {
	public function __construct($arguments) {
		parent::__construct($arguments);
		$query = Database::getConnection($this->cgttConection, $this->cgttConectionKey)
			->select('tags')
			->fields('tags', array('idtag',
				'valor',
			))->orderBy('valor', 'ASC');

		$this->source = new MigrateSourceSQL($query, array(), NULL, array(map_joinable => FALSE));

		// Set up our destination - terms in the migrate_example_beer_styles
		// vocabulary (note that we pass the machine name of the vocabulary).
		$this->destination =
			new MigrateDestinationTerm('tag');

		$this->map = new MigrateSQLMap($this->machineName,
			array(
				'idtag' => array('type' => 'int',
					'length' => 255,
					'not null' => TRUE,
					'description' => 'Tag ID',
				)
			),
			MigrateDestinationTerm::getKeySchema()
		);

		$this->addFieldMapping('name', 'valor');
	}

}
